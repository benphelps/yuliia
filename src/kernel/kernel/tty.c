#include <kernel/tty.h>
#include <common.h>
#include <arch/ports.h>

/* Declaration of private functions */
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int get_cursor_offset();
void set_cursor_offset(int offset);

void terminal_initialize()
{
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
    terminal_global_color = terminal_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    for (i = 0; i < screen_size; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = terminal_global_color;
    }
    set_cursor_offset(get_offset(0, 0));
}

void reset_terminal_color()
{
    terminal_global_color = terminal_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void set_terminal_color(uint8_t fg, uint8_t bg)
{
    terminal_global_color = terminal_color(fg, bg);
}

void terminal_putentryat(char *message, uint8_t color, int col, int row)
{
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
    {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0)
    {
        offset = terminal_putchar(message[i++], col, row, color);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void terminal_put_color(char data, uint8_t color)
{
    char str[2] = {data, '\0'};
    terminal_putentryat(str, color, -1, -1);
}

void terminal_write_color(char *data, uint8_t color)
{
    terminal_putentryat(data, color, -1, -1);
}

void terminal_writestring_color(char *data, uint8_t color)
{
    terminal_putentryat(data, color, -1, -1);
}

void terminal_backspace()
{
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    terminal_putchar(0x08, col, row, WHITE_ON_BLACK);
}

int terminal_putchar(char c, int col, int row, char attr)
{
    uint8_t *vidmem = (uint8_t *)VIDEO_ADDRESS;
    if (!attr)
        attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS)
    {
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_offset();

    if (c == '\n')
    {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else if (c == 0x08)
    { /* Backspace */
        vidmem[offset] = ' ';
        vidmem[offset + 1] = attr;
    }
    else
    {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2)
    {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            memcpy((uint8_t *)(get_offset(0, i - 1) + VIDEO_ADDRESS),
                   (uint8_t *)(get_offset(0, i) + VIDEO_ADDRESS),
                   MAX_COLS * 2);

        /* Blank last line */
        char *last_line = (char *)(get_offset(0, MAX_ROWS - 1) + (uint8_t *)VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++)
            last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

void terminal_write_dec(u32int n)
{
    if (n == 0)
    {
        terminal_put('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0)
    {
        c2[i--] = c[j++];
    }
    terminal_write(c2);
}

void terminal_write_hex(u32int n)
{
    s32int tmp;

    terminal_write("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }

        if (tmp >= 0xA)
        {
            noZeroes = 0;
            terminal_put(tmp - 0xA + 'A');
        }
        else
        {
            noZeroes = 0;
            terminal_put(tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        terminal_put(tmp - 0xA + 'A');
    }
    else
    {
        terminal_put(tmp + '0');
    }
}

int get_cursor_offset()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }

void terminal_splash()
{
    set_terminal_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write("\n __   __     _ _ _       \n");
    terminal_write(" \\ \\ / /   _| (_|_) __ _ \n");
    terminal_write("  \\ V / | | | | | |/ _` |\n");
    terminal_write("   | || |_| | | | | (_| |\n");
    terminal_write("   |_| \\__,_|_|_|_|\\__,_|\n\n");
    terminal_write("   Project Yuliia v0.0.1\n\n");
    reset_terminal_color();
}
