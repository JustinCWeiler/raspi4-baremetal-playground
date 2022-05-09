SECTIONS {
	.text 0x80000 : {
		bin/boot.o(.text)
		bin/cboot.o(.text)
		. = ALIGN(16);
		_boot_start_ = .;
		bin/main.o(.text)
		*(.text*)
	}

	.data : {
		. = ALIGN(8);
		*(.data*)
	}

	_boot_end_ = .;
}
