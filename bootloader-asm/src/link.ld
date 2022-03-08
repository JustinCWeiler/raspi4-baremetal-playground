SECTIONS {
	.text 0x80000 : {
		*(.text.boot)
		*(.text)
		*(.text.lib)
		*(.text*) /* catch all sections we missed */
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}
}
