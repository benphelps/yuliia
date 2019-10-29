#/bin/bash
make clean && make all
/usr/local/bin/qemu-system-i386 -cdrom /Users/phelps/Projects/yuliia/build/yuliia.iso
