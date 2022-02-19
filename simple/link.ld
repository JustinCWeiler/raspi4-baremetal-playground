SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		*(.text*)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}
}
