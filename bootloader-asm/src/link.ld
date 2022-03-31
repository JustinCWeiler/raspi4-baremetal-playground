SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		_boot_start_ = .;
		*(.text.bootmain)
		*(.text)
	}

	.data : {
		*(.data)
	}

	.text : {
		*(.text.lib)
	}

	.data : {
		*(.data.lib)
	}

	.text : {
		*(.text*)
	}

	.data : {
		*(.data*)
	}

	/* not needed probably?
	.bss : {
		*(.bss)
	}
	*/

	_boot_end_ = .;

	_boot_size_ = (_boot_end_ - _boot_start_) >> 2;
}
