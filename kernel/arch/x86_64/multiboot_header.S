.section .multiboot_header
header_start:
    .long 0xe85250d6                # magic number (multiboot 2)
    .long 0                         # architecture 0 (protected mode i386)
    .long header_end - header_start # header length

    # checksum
    .long 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    # required end tag
    .word 0     # type
    .word 0     # flags
    .long 8     # size
header_end:
