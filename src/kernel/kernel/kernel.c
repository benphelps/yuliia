#include <common.h>
#include <arch/isr.h>
#include <arch/timer.h>
#include <arch/multiboot.h>
#include <drivers/keyboard.h>

void kernel_main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    terminal_initialize();
    terminal_splash();

    init_descriptor_tables();
    asm volatile("sti");

    init_timer(50);
    init_keyboard_driver();

    set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write("    ");
    for (size_t bg = 0; bg <= 15; bg++)
    {
        terminal_write_hex(bg);
        terminal_write(" ");
    }
    terminal_write("\n");
    for (size_t fg = 0; fg <= 15; fg++)
    {
        set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
        terminal_write_hex(fg);
        terminal_write(" ");
        for (size_t bg = 0; bg <= 15; bg++)
        {
            set_terminal_color(fg, bg);
            terminal_write_hex(fg);
            set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
            terminal_write(" ");
        }
        terminal_write("\n");
    }
    terminal_write("\n");

    set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write("\n> ");
    reset_terminal_color();

    UNUSED(mboot_ptr);
    UNUSED(initial_stack);
}

int user_input(char *input, uint8_t scancode)
{
    if (scancode == ENTER)
    {
        set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
        terminal_write("\n> ");
        reset_terminal_color();
        return 1;
    }
    terminal_put(keyboard_getchar());
    UNUSED(input);
    return 0;
}
