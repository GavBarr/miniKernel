

section .header
align 4

MAGIC_NUMBER: dd 0x1BADB002
FLAGS: dd 0
CHECKSUM: dd -(0x1BADB002 + 0)


section .bss


section .text
