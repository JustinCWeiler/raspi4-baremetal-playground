SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		*(.text*)
	}

	.data : {
		. = ALIGN(8);
		*(.data)
		. = ALIGN(8);
	}

	.bss : {
		. = ALIGN(8);
		__bss_start__ = .;
		*(.bss)
		. = ALIGN(8);
		__bss_end__ = .;
	}
}
