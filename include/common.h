#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/cdefs.h>
#include <arch/types.h>
#include <kernel/debug.h>
#include <kernel/tty.h>

#define UNUSED(x) (void)(x)
#define LOAD_MEMORY_ADDRESS 0xC0000000
#define K 1024
#define M (1024*K)
#define G (1024*M)
