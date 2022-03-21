SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
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
}
