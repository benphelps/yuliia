#!/bin/bash
export CC=/usr/local/bin/gcc-9
export LD=/usr/local/bin/gcc-9
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# clean up old source and bin dirs
# rm -rf $HOME/opt
rm -rf $HOME/src

# make new source and bin dirs
# mkdir $HOME/opt
mkdir $HOME/src

# # download and compile binutils
# cd $HOME/src
# curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.gz
# tar xf binutils-2.32.tar.gz
# mkdir build-binutils
# cd build-binutils
# ../binutils-2.32/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
# make -j 13
# make install

# # download and compile GCC
# cd $HOME/src
# curl -O https://ftp.gnu.org/gnu/gcc/gcc-8.3.0/gcc-8.3.0.tar.gz
# tar xf gcc-8.3.0.tar.gz
# cd gcc-8.3.0/
# ./contrib/download_prerequisites
# cd $HOME/src
# mkdir build-gcc
# cd build-gcc
# ../gcc-8.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
# make all-gcc -j 13
# make all-target-libgcc -j 13
# make install-gcc
# make install-target-libgcc

# download and compile gdb
cd $HOME/src
curl -O https://ftp.gnu.org/gnu/gdb/gdb-8.3.1.tar.gz
tar xf gdb-8.3.1.tar.gz
mkdir build-gdb
cd build-gdb
../gdb-8.3.1/configure --target=$TARGET --prefix="$PREFIX"
make -j 13
make install

# # download and compile objconv
# cd $HOME/src
# git clone https://github.com/vertis/objconv.git
# cd objconv/
# /usr/local/bin/g++-9 -o objconv -O2 src/*.cpp -Wno-narrowing
# cp objconv $PREFIX/

# # download and compile grub
# cd $HOME/src
# wget https://ftp.gnu.org/gnu/grub/grub-2.04.tar.xz
# tar xf grub-2.04.tar.xz
# mkdir build-grub
# cd build-grub
# gsed -i 's/FILE_TYPE_NO_DECOMPRESS/GRUB_FILE_TYPE_NO_DECOMPRESS/g' ../grub-2.04/grub-core/osdep/generic/blocklist.c
# ../grub-2.04/configure --disable-werror TARGET_CC=$TARGET-gcc TARGET_OBJCOPY=$TARGET-objcopy TARGET_STRIP=$TARGET-strip TARGET_NM=$TARGET-nm TARGET_RANLIB=$TARGET-ranlib --target=$TARGET --prefix="$PREFIX"
# make -j 13
# make install

# # download and compile qemu, this requires a patched homebrew formula
# brew install benphelps/homebrew-core/qemu --build-from-source
