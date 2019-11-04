#include <kernel/tty.h>
#include <stdio.h>

void putchar(int ic)
{
    char c = (char)ic;
    terminal_put(c);
}
