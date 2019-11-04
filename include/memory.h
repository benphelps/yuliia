#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr);

#endif
