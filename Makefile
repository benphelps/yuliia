export PATH := ${HOME}/opt/cross/bin:$(PATH)
TARGET=i686-elf
GCC = ${TARGET}-gcc
CC = ${TARGET}-gcc
LD = ${TARGET}-ld
AS = ${TARGET}-as
GDB = ${TARGET}-gdb
OBJCPY = ${TARGET}-objcopy
QEMU = /usr/local/bin/qemu-system-i386
NASM = /usr/local/bin/nasm
GRUB = grub-mkrescue
XORRISO = /usr/local/bin/xorriso
CFLAGS = -g -ffreestanding -Wall -Wextra -Iinclude
CPPFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude
CLFLAGS = ${CFLAGS} -nostdlib
SRCDIR = src
BUILDDIR = build

SOURCES := $(filter-out src/kernel/kernel/kernel.c, $(shell find src -type f -name '*.c') $(shell find src/kernel -type f -name '*.asm') $(shell find src/kernel -type f -name '*.s'))
HEADERS := $(filter-out src/kernel/kernel/kernel.h, $(shell find src -type f -name '*.h'))
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
OBJECTS := $(OBJECTS:$(SRCDIR)/%.asm=$(BUILDDIR)/%.o)
OBJECTS := $(OBJECTS:$(SRCDIR)/%.s=$(BUILDDIR)/%.o)

default: all

# full bootdisk, bootloader + kernel
${BUILDDIR}/bootdisk/yuliia.bin: ${BUILDDIR}/bootloader/crti.o ${BUILDDIR}/bootloader/crtbegin.o ${BUILDDIR}/bootloader/boot.o ${BUILDDIR}/kernel/kernel.o ${OBJECTS} ${BUILDDIR}/bootloader/crtend.o  ${BUILDDIR}/bootloader/crtn.o
	mkdir -p $(@D)
	mkdir -p ${BUILDDIR}/iso/boot/grub
	$(GCC) -T ${SRCDIR}/linker.ld -o $@ ${CLFLAGS} $^ ${objects} -lgcc
	cp ${SRCDIR}/bootloader/grub.cfg ${BUILDDIR}/iso/boot/grub/grub.cfg
	cp $@ ${BUILDDIR}/iso/boot/yuliia.bin
	${GRUB} --xorriso=${XORRISO} -o ${BUILDDIR}/yuliia.iso ${BUILDDIR}/iso
	${OBJCPY} --only-keep-debug ${BUILDDIR}/bootdisk/yuliia.bin ${BUILDDIR}/bootdisk/yuliia.sym

# kernel object
${BUILDDIR}/kernel/kernel.o: ${SRCDIR}/kernel/kernel/kernel.c
	mkdir -p $(@D)
	${GCC} ${CFLAGS} -c $< -o $@ -lgcc

# kernel asm
${BUILDDIR}/kernel/%.o: ${SRCDIR}/kernel/%.asm
	mkdir -p $(@D)
	${NASM} $< -f elf32 -o $@

# kernel objects
${BUILDDIR}/kernel/%.o: ${SRCDIR}/kernel/%.c
	mkdir -p $(@D)
	${GCC} -c $< -o $@ ${CFLAGS}

# kernel objects
${BUILDDIR}/kernel/%.o: ${SRCDIR}/kernel/%.cpp
	mkdir -p $(@D)
	${GCC} -c $< -o $@ ${CPPFLAGS}

# bootloader asm objects
${BUILDDIR}/kernel/%.o: ${SRCDIR}/kernel/%.s
	mkdir -p $(@D)
	$(NASM) $< -f elf32 -o $@

# bootloader objects
${BUILDDIR}/bootloader/%.o: ${SRCDIR}/bootloader/%.s
	mkdir -p $(@D)
	$(AS) $< -o $@

${BUILDDIR}/bootloader/crti.s.o ${BUILDDIR}/bootloader/crtn.s.o:
	${GCC} -MD -c $F -o $@ ${CFLAGS}

${BUILDDIR}/bootloader/crtbegin.o ${BUILDDIR}/bootloader/crtend.o:
	OBJ=`$(CC) $(CFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.PHONY: clean run all setup

all: ${BUILDDIR}/bootdisk/yuliia.bin

run: ${BUILDDIR}/bootdisk/yuliia.bin
	${QEMU} -cdrom ${BUILDDIR}/yuliia.iso -m 512M

debug: all
	${QEMU} -s -S -cdrom ${BUILDDIR}/yuliia.iso
    #  & ${GDB} -ex "file ${BUILDDIR}/kernel/kernel.sym" -ex "set arch i386\:x86-64" -ex "target remote localhost:1234"

clean:
	rm -rf ${BUILDDIR}
