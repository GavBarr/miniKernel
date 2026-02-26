#!/bin/bash
set -e

rm -f *.o kernel.bin mykernel.iso

nasm -f elf32 start.asm        -o start.o
nasm -f elf32 gdt/gdt_flush.asm -o gdt/gdt_flush.o
nasm -f elf32 idt/isr.asm      -o idt/isr_stubs.o
nasm -f elf32 task/switch.asm  -o task/switch.o

echo "Compiling..."

CFLAGS="-m32 -ffreestanding -fno-pie -O0"
CC=i686-elf-gcc
AS=i686-elf-as
LD=i686-elf-ld

$CC $CFLAGS -c kernel.c             -o kernel.o
$CC $CFLAGS -c gdt/gdt.c            -o gdt/gdt.o
$CC $CFLAGS -c idt/idt.c            -o idt/idt.o
$CC $CFLAGS -c drivers/keyboard.c   -o drivers/keyboard.o
$CC $CFLAGS -c drivers/screen.c     -o drivers/screen.o
$CC $CFLAGS -c drivers/ramdisk.c    -o drivers/ramdisk.o
$CC $CFLAGS -c drivers/ide_ata_driver.c -o drivers/ide_ata_driver.o
$CC $CFLAGS -c kernel_shell/parser.c   -o kernel_shell/parser.o
$CC $CFLAGS -c fs/superblock.c      -o fs/superblock.o
$CC $CFLAGS -c fs/bitmap.c          -o fs/bitmap.o
$CC $CFLAGS -c fs/file.c            -o fs/file.o
$CC $CFLAGS -c fs/inode.c           -o fs/inode.o
$CC $CFLAGS -c fs/directory.c       -o fs/directory.o
$CC $CFLAGS -c fs/fs.c              -o fs/fs.o
$CC $CFLAGS -c mem_alloc/mm.c       -o mem_alloc/mm.o
$CC $CFLAGS -c mem_alloc/mem_alloc.c -o mem_alloc/mem_alloc.o
$CC $CFLAGS -c mem_alloc/bitmap.c   -o mem_alloc/bitmap.o
$CC $CFLAGS -c mem_alloc/heap.c     -o mem_alloc/heap.o
$CC $CFLAGS -c paging/paging.c      -o paging/paging.o
$CC $CFLAGS -c debug/debug.c        -o debug/debug.o
$CC $CFLAGS -c kernel_shell/shell.c -o kernel_shell/shell.o
$CC $CFLAGS -c include/strcompare.c -o include/strcompare.o
$CC $CFLAGS -c include/strlength.c  -o include/strlength.o
$CC $CFLAGS -c include/memset.c     -o include/memset.o
$CC $CFLAGS -c include/memcopy.c    -o include/memcopy.o
$CC $CFLAGS -c include/device_manager.c -o include/device_manager.o
$CC $CFLAGS -c task/sched.c         -o task/sched.o
$CC $CFLAGS -c task/task.c          -o task/task.o

echo "Linking..."

$LD -T linker.ld -o kernel.bin \
  start.o kernel.o gdt/gdt.o gdt/gdt_flush.o idt/idt.o idt/isr_stubs.o task/switch.o \
    drivers/keyboard.o drivers/screen.o drivers/ramdisk.o drivers/ide_ata_driver.o kernel_shell/parser.o \
      fs/superblock.o fs/bitmap.o fs/file.o fs/inode.o fs/directory.o fs/fs.o \
        mem_alloc/mm.o mem_alloc/mem_alloc.o mem_alloc/bitmap.o mem_alloc/heap.o \
          paging/paging.o debug/debug.o kernel_shell/shell.o include/strcompare.o include/strlength.o include/memset.o \
            include/device_manager.o include/memcopy.o \
              task/sched.o task/task.o

              echo "Creating ISO..."

              #cp kernel.bin iso/boot/

              echo "Build complete!"
