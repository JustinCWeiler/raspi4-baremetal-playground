extern char _boot_start_;
extern char _boot_end_;

void _cstart(void) {
	// get end of memory

	while (1) asm volatile ("wfe");
}
