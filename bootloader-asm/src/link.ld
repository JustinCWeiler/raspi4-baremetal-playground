SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		_boot_start_ = .;
		*(.text.bootmain)
		*(.text)
	}

	.data : {
		. = ALIGN(8);
		*(.data)
	}

	.text : {
		. = ALIGN(16);
		*(.text.lib)
	}

	.data : {
		. = ALIGN(8);
		*(.data.lib)
	}

	.text : {
		. = ALIGN(16);
		*(.text*)
	}

	.data : {
		. = ALIGN(8);
		*(.data*)
	}

	_boot_end_ = .;
}
