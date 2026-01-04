

section .header
align 4

MAGIC_NUMBER: dd 0x1BADB002
FLAGS: dd 0
CHECKSUM: dd -(0x1BADB002 + 0)


section .bss
buffer: resb 16384
top:

extern kernel_main

section .text
global _start
_start: 
	mov esp, top
	call kernel_main
	cli
	hlt



