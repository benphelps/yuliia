#include <memory.h>

extern uint32_t _kernel_end;
uint32_t kernel_end = (uint32_t)&_kernel_end;

/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr)
{
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (kernel_end & 0xFFFFF000))
    {
        kernel_end &= 0xFFFFF000;
        kernel_end += 0x1000;
    }

    /* Save also the physical address */
    if (phys_addr)
        *phys_addr = kernel_end;

    uint32_t ret = kernel_end;
    kernel_end += size; /* Remember to increment the pointer */
    return ret;
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
