#define GPIO_BASE ( PERIPHERAL_BASE + 0x00200000 )
#define GPIO_SET1 ( (volatile uint32_t*)( GPIO_BASE + 0x20 ) )
#define GPIO_CLR1 ( (volatile uint32_t*)( GPIO_BASE + 0x2C ) )

// WRITE
static void gpio_act_on( void ) {
	// WRITE
	*GPIO_SET1 = 1 << 10;
}

// WRITE
static void gpio_act_off( void ) {
	// WRITE
	*GPIO_CLR1 = 1 << 10;
}
