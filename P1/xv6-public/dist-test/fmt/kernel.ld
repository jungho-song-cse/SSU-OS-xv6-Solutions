8500 /* Simple linker script for the JOS kernel.
8501    See the GNU ld 'info' manual ("info ld") to learn the syntax. */
8502 
8503 OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
8504 OUTPUT_ARCH(i386)
8505 ENTRY(_start)
8506 
8507 SECTIONS
8508 {
8509 	/* Link the kernel at this address: "." means the current address */
8510         /* Must be equal to KERNLINK */
8511 	. = 0x80100000;
8512 
8513 	.text : AT(0x100000) {
8514 		*(.text .stub .text.* .gnu.linkonce.t.*)
8515 	}
8516 
8517 	PROVIDE(etext = .);	/* Define the 'etext' symbol to this value */
8518 
8519 	.rodata : {
8520 		*(.rodata .rodata.* .gnu.linkonce.r.*)
8521 	}
8522 
8523 	/* Include debugging information in kernel memory */
8524 	.stab : {
8525 		PROVIDE(__STAB_BEGIN__ = .);
8526 		*(.stab);
8527 		PROVIDE(__STAB_END__ = .);
8528 	}
8529 
8530 	.stabstr : {
8531 		PROVIDE(__STABSTR_BEGIN__ = .);
8532 		*(.stabstr);
8533 		PROVIDE(__STABSTR_END__ = .);
8534 	}
8535 
8536 	/* Adjust the address for the data segment to the next page */
8537 	. = ALIGN(0x1000);
8538 
8539 	/* Conventionally, Unix linkers provide pseudo-symbols
8540 	 * etext, edata, and end, at the end of the text, data, and bss.
8541 	 * For the kernel mapping, we need the address at the beginning
8542 	 * of the data section, but that's not one of the conventional
8543 	 * symbols, because the convention started before there was a
8544 	 * read-only rodata section between text and data. */
8545 	PROVIDE(data = .);
8546 
8547 
8548 
8549 
8550 	/* The data segment */
8551 	.data : {
8552 		*(.data)
8553 	}
8554 
8555 	PROVIDE(edata = .);
8556 
8557 	.bss : {
8558 		*(.bss)
8559 	}
8560 
8561 	PROVIDE(end = .);
8562 
8563 	/DISCARD/ : {
8564 		*(.eh_frame .note.GNU-stack)
8565 	}
8566 }
8567 
8568 
8569 
8570 
8571 
8572 
8573 
8574 
8575 
8576 
8577 
8578 
8579 
8580 
8581 
8582 
8583 
8584 
8585 
8586 
8587 
8588 
8589 
8590 
8591 
8592 
8593 
8594 
8595 
8596 
8597 
8598 
8599 
