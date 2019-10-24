#include <arch/ports.h>

/**
 * Read a byte from the specified port
 */
uint8_t port_byte_in(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "dN"(port));
    return ret;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    asm volatile("out %%al, %%dx"
                 :
                 : "a"(data), "d"(port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;
    asm("in %%dx, %%ax"
        : "=a"(result)
        : "d"(port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    asm volatile("out %%ax, %%dx"
                 :
                 : "a"(data), "d"(port));
}
