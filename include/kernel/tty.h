#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <arch/types.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t terminal_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/* Public kernel API */
uint8_t terminal_global_color;
void terminal_initialize();
void terminal_putentryat_color(char *message, uint8_t color, int col, int row);
void terminal_write_color(char *data, uint8_t color);
void terminal_put_color(char data, uint8_t color);
void terminal_writestring_color(char *data, uint8_t color);
void terminal_write_hex(u32int n);
void terminal_write_dec(u32int n);
void terminal_backspace();
void reset_terminal_color();
void set_terminal_color(uint8_t fg, uint8_t bg);
int terminal_putchar(char c, int col, int row, char attr);
void terminal_splash();
#define terminal_put(data) terminal_put_color(data, terminal_global_color);
#define terminal_write(data) terminal_write_color(data, terminal_global_color);
#define terminal_writestring(data) terminal_writestring_color(data, terminal_global_color);

#endif
