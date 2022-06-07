#define TIMER_BASE	(PERIPHERAL_BASE+0x3000)
#define TIMER_LO	((volatile uint32_t*)(TIMER_BASE+0x04))
#define TIMER_HI	((volatile uint32_t*)(TIMER_BASE+0x08))

// READ
static uint64_t timer_get_usec() {
	uint64_t hi = *TIMER_HI;
	uint64_t lo = *TIMER_LO;
	return (hi << 32) | lo;
}
