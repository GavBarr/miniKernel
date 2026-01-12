#!/bin/bash
set -e  # exit immediately if any command fails

rm -f *.o kernel.bin mykernel.iso

nasm -f elf32 start.asm -o start.o
nasm -f elf32 gdt_flush.asm -o gdt_flush.o
nasm -f elf32 isr.asm -o isr_stubs.o

echo "Compiling..."
CFLAGS="-m32 -ffreestanding -nostdlib -fno-pie"

gcc $CFLAGS -c kernel.c     -o kernel.o
gcc $CFLAGS -c gdt.c        -o gdt.o
gcc $CFLAGS -c idt.c        -o idt.o
gcc $CFLAGS -c keyboard.c   -o keyboard.o
gcc $CFLAGS -c mm.c         -o mm.o
gcc $CFLAGS -c mem_alloc.c  -o mem_alloc.o
gcc $CFLAGS -c bitmap.c     -o bitmap.o
gcc $CFLAGS -c paging.c     -o paging.o
gcc $CFLAGS -c debug.c      -o debug.o

echo "Linking..."
ld -m elf_i386 -T linker.ld \
  -o kernel.bin \
  start.o kernel.o gdt.o gdt_flush.o idt.o isr_stubs.o \
  keyboard.o mm.o mem_alloc.o bitmap.o paging.o debug.o

echo "CreatingISO..."
cp kernel.bin iso/boot/
grub-mkrescue -o mykernel.iso iso/

echo "Build complete"

