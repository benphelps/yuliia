#include <common.h>
#include <memory.h>

extern uint32_t _kernel_end;
uint32_t placement_addres = (uint32_t)&_kernel_end;

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr)
{
    if (align == 1 && (placement_addres & 0xFFFFF000))
    {
        placement_addres &= 0xFFFFF000;
        placement_addres += 0x1000;
    }

    if (phys_addr)
    {
        *phys_addr = placement_addres;
    }

    uint32_t ptr = placement_addres;
    placement_addres += size; /* Remember to increment the pointer */
    return ptr;
}

int memcmp(const void *aptr, const void *bptr, size_t size)
{
    const unsigned char *a = (const unsigned char *)aptr;
    const unsigned char *b = (const unsigned char *)bptr;
    for (size_t i = 0; i < size; i++)
    {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

void *memcpy(void *restrict dstptr, const void *restrict srcptr, size_t size)
{
    unsigned char *dst = (unsigned char *)dstptr;
    const unsigned char *src = (const unsigned char *)srcptr;
    for (size_t i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}

void *memmove(void *dstptr, const void *srcptr, size_t size)
{
    unsigned char *dst = (unsigned char *)dstptr;
    const unsigned char *src = (const unsigned char *)srcptr;
    if (dst < src)
    {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    }
    else
    {
        for (size_t i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];
    }
    return dstptr;
}

void *memset(void *bufptr, int value, size_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}
