#/bin/bash
make clean && make debug
/usr/local/bin/qemu-system-i386 -s -S -cdrom /Users/phelps/Projects/yuliia/build/yuliia.iso
