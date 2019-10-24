# Yuliia

Yuliia is a toy operating system.

## Plans

This is mostly just an exercise in C. I plan on writing userspace applications in another higher level language like Lua.

## Building the cross compiler

The included `build_gcc.sh` assumes you're building under macOS but can easily be adapted for any other \*nix type OS.

## Building

Once the cross compiler is installed as described in `build_gcc.sh`, simply run `make run` to compile and run the OS in QEMU.
