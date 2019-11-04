#include <arch/isr.h>
#include <arch/multiboot.h>
#include <arch/timer.h>
#include <common.h>
#include <drivers/keyboard.h>

void terminal_demo();
extern uint32_t kernel_virtual_end;
extern uint32_t kernel_virtual_start;

void kernel_main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    terminal_initialize();
    reset_terminal_color();
    terminal_splash();

    init_descriptor_tables();
    asm volatile("sti");
    init_timer(50);
    init_keyboard_driver();

    // terminal_demo();

    printf("initial_stack: 0x%x\n", (uint32_t)&initial_stack);
    printf("kernel_virtual_start: 0x%x\n", (uint32_t)&kernel_virtual_start);
    printf("kernel_virtual_end: 0x%x\n", (uint32_t)&kernel_virtual_end);
    printf("kernel_virtual_size: 0x%x\n", (uint32_t)&kernel_virtual_end - (uint32_t)&kernel_virtual_start);

    for (size_t i = 0; i < 6; i++)
    {
        uint32_t phys_addr;
        uint32_t page = kmalloc(sizeof(char[1024 * 12]), 1, &phys_addr);
        printf("page: 0x%x, physical address: 0x%x\n", page, phys_addr);
    }

    set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write("\n> ");
    reset_terminal_color();

    UNUSED(mboot_ptr);
    UNUSED(initial_stack);
    for (;;)
    {
        // nothin
    }
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

void terminal_demo()
{
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
}
