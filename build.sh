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
gcc $CFLAGS -c drivers/keyboard.c   -o drivers/keyboard.o
gcc $CFLAGS -c drivers/screen.c   -o drivers/screen.o
gcc $CFLAGS -c drivers/ramdisk.c   -o drivers/ramdisk.o
gcc $CFLAGS -c drivers/ide_ata_driver.c   -o drivers/ide_ata_driver.o
gcc $CFLAGS -c kernel_shell/parser.c   -o kernel_shell/parser.o
gcc $CFLAGS -c fs/superblock.c   -o fs/superblock.o
gcc $CFLAGS -c fs/bitmap.c   -o fs/bitmap.o
gcc $CFLAGS -c fs/inode.c   -o fs/inode.o
gcc $CFLAGS -c fs/file.c   -o fs/file.o
gcc $CFLAGS -c mem_alloc/mm.c         -o mem_alloc/mm.o
gcc $CFLAGS -c mem_alloc/mem_alloc.c  -o mem_alloc/mem_alloc.o
gcc $CFLAGS -c mem_alloc/bitmap.c     -o mem_alloc/bitmap.o
gcc $CFLAGS -c mem_alloc/heap.c     -o mem_alloc/heap.o
gcc $CFLAGS -c paging/paging.c     -o paging/paging.o
gcc $CFLAGS -c debug/debug.c      -o debug/debug.o
gcc $CFLAGS -c kernel_shell/shell.c      -o kernel_shell/shell.o
gcc $CFLAGS -c include/strcompare.c      -o include/strcompare.o
gcc $CFLAGS -c include/strlength.c      -o include/strlength.o
gcc $CFLAGS -c include/memset.c      -o include/memset.o
gcc $CFLAGS -c include/device_manager.c      -o include/device_manager.o

echo "Linking..."
ld -m elf_i386 -T linker.ld \
  -o kernel.bin \
  start.o kernel.o gdt/gdt.o gdt/gdt_flush.o idt/idt.o idt/isr_stubs.o \
  drivers/keyboard.o drivers/screen.o drivers/ramdisk.o drivers/ide_ata_driver.o  kernel_shell/parser.o \
  fs/superblock.o fs/bitmap.o fs/file.o fs/inode.o\
  mem_alloc/mm.o mem_alloc/mem_alloc.o mem_alloc/bitmap.o \
  mem_alloc/heap.o paging/paging.o debug/debug.o kernel_shell/shell.o include/strcompare.o include/strlength.o include/memset.o\
  include/device_manager.o

echo "CreatingISO..."
cp kernel.bin iso/boot/
grub-mkrescue -o mykernel.iso iso/

echo "Build complete"

