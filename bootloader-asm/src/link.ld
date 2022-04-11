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

	.bss : {
		*(.bss)
	}

	_boot_end_ = .;
}
