#define GPIO_FSEL1 ((volatile uint32_t*)(GPIO_BASE+0x04))

#define AUX_BASE	(PERIPHERAL_BASE+0x00215000)
#define AUX_MU_BASE	(AUX_BASE+0x40)

#define AUX_ENABLES	((volatile uint32_t*)(AUX_BASE+0x04))

#define AUX_MU_IO	((volatile uint32_t*)(AUX_MU_BASE+0x00))
#define AUX_MU_IER	((volatile uint32_t*)(AUX_MU_BASE+0x04))
#define AUX_MU_IIR	((volatile uint32_t*)(AUX_MU_BASE+0x08))
#define AUX_MU_LCR	((volatile uint32_t*)(AUX_MU_BASE+0x0C))
#define AUX_MU_CNTL	((volatile uint32_t*)(AUX_MU_BASE+0x20))
#define AUX_MU_STAT	((volatile uint32_t*)(AUX_MU_BASE+0x24))
#define AUX_MU_BAUD	((volatile uint32_t*)(AUX_MU_BASE+0x28))

#define B115200 541
#define MU_FUNC 0b010
#define RX_OFFSET (5*3)
#define TX_OFFSET (4*3)

// handles memory barriers
static void uart_init(void) {
	// set gpio pins
	// LAST READ GPIO
	uint32_t val = *GPIO_FSEL1;
	val &= ~( (0b111 << RX_OFFSET) | (0b111 << TX_OFFSET) );
	val |= (MU_FUNC << RX_OFFSET) | (MU_FUNC << TX_OFFSET);
	// FIRST WRITE GPIO
	MB_RDWR;
	*GPIO_FSEL1 = val;

	// enable uart
	// LAST READ AUX
	val = *AUX_ENABLES;
	val |= 1;
	// FIRST WRITE AUX
	MB_RDWR;
	*AUX_ENABLES = val;

	// configure
	// disable tx and rx
	// FIRST WRITE MU
	MB_WR;
	*AUX_MU_CNTL = 0;

	// clear FIFOs
	*AUX_MU_IIR = 0b11 << 1;

	// disable interrupts
	*AUX_MU_IER = 0;

	// set baudrate
	*AUX_MU_BAUD = B115200;

	// set to 8 bit mode
	*AUX_MU_LCR = 0b11;

	// reenable tx and rx
	*AUX_MU_CNTL = 0b11;
}

// READ then WRITE
static void uart_write(uint8_t data) {
	// READ
	while ( !((*AUX_MU_STAT >> 1) & 1) ) ;

	// WRITE
	*AUX_MU_IO = data;
}

// READ
static uint8_t uart_read(void) {
	// READ
	while ( !(*AUX_MU_STAT & 1) ) ;

	// READ
	return *AUX_MU_IO;
}
