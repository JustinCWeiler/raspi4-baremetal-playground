SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		_boot_start_ = .;
		*(.text.bootmain)
		*(.text*)
	}

	.data : {
		. = ALIGN(8);
		*(.data*)
	}

	_boot_end_ = .;
}
