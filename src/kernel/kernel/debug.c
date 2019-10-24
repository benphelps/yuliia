#include <kernel/tty.h>
#include <arch/types.h>

extern void panic(char *message, char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    terminal_write("PANIC(");
    terminal_write(message);
    terminal_write(") at ");
    terminal_write(file);
    terminal_write(":");
    terminal_write_dec(line);
    terminal_write("\n");
    // Halt by going into an infinite loop.
    for (;;)
        ;
}

extern void panic_assert(char *file, u32int line, char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    terminal_write("ASSERTION-FAILED(");
    terminal_write(desc);
    terminal_write(") at ");
    terminal_write(file);
    terminal_write(":");
    terminal_write_dec(line);
    terminal_write("\n");
    // Halt by going into an infinite loop.
    for (;;)
        ;
}
