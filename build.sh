#!/bin/bash
set -e  # exit immediately if any command fails

rm -f *.o kernel.bin mykernel.iso

nasm -f elf32 start.asm -o start.o
nasm -f elf32 gdt/gdt_flush.asm -o gdt/gdt_flush.o
nasm -f elf32 idt/isr.asm -o idt/isr_stubs.o

echo "Compiling..."
CFLAGS="-m32 -ffreestanding -nostdlib -fno-pie"

gcc $CFLAGS -c kernel.c     -o kernel.o
gcc $CFLAGS -c gdt/gdt.c        -o gdt/gdt.o
gcc $CFLAGS -c idt/idt.c        -o idt/idt.o
gcc $CFLAGS -c keyboard.c   -o keyboard.o
gcc $CFLAGS -c mem_alloc/mm.c         -o mem_alloc/mm.o
gcc $CFLAGS -c mem_alloc/mem_alloc.c  -o mem_alloc/mem_alloc.o
gcc $CFLAGS -c mem_alloc/bitmap.c     -o mem_alloc/bitmap.o
gcc $CFLAGS -c paging/paging.c     -o paging/paging.o
gcc $CFLAGS -c debug/debug.c      -o debug/debug.o

echo "Linking..."
ld -m elf_i386 -T linker.ld \
  -o kernel.bin \
  start.o kernel.o gdt/gdt.o gdt/gdt_flush.o idt/idt.o idt/isr_stubs.o \
  keyboard.o mem_alloc/mm.o mem_alloc/mem_alloc.o mem_alloc/bitmap.o paging/paging.o debug/debug.o

echo "CreatingISO..."
cp kernel.bin iso/boot/
grub-mkrescue -o mykernel.iso iso/

echo "Build complete"

