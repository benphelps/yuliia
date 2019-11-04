#include <common.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < length; i++)
        putchar(bytes[i]);
    return true;
}

// void printf(const char *format, ...)
// {
//     va_list parameters;
//     va_start(parameters, format);
//     while (*format != '\0')
//     {
//         if (*format == '%')
//         {
//             format++;
//             if (*format == '%')
//             {
//                 putchar('%');
//                 format++;
//             }
//             else if (*format == 'c')
//             {
//                 char c = va_arg(parameters, int);
//                 putchar(c);
//                 format++;
//             }
//             else if (*format == 's')
//             {
//                 char *chars = va_arg(parameters, char *);
//                 size_t len = sizeof(chars);
//                 print(chars, len);
//                 format += len;
//             }
//             else
//             {
//                 PANIC("NYI");
//             }
//         }
//         else
//         {
//             putchar((int)*format);
//             format++;
//         }
//     }
//     va_end(parameters);
// }

void printf(const char *format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    while (*format != '\0')
    {
        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            print(format, amount);
            format += amount;
            continue;
        }

        const char *format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char)va_arg(parameters, int /* char promotes to int */);
            print(&c, sizeof(c));
        }
        else if (*format == 's')
        {
            format++;
            const char *str = va_arg(parameters, const char *);
            size_t len = strlen(str);
            print(str, len);
        }
        else if (*format == 'x')
        {
            format++;
            uint32_t n = va_arg(parameters, uint32_t);
            uint32_t tmp;
            char noZeroes = 5;
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
                    putchar(tmp - 0xA + 'A');
                }
                else
                {
                    noZeroes = 0;
                    putchar(tmp + '0');
                }
            }
            tmp = n & 0xF;
            if (tmp >= 0xA)
            {
                putchar(tmp - 0xA + 'A');
            }
            else
            {
                putchar(tmp + '0');
            }
        }
        else
        {
            format = format_begun_at;
            size_t len = strlen(format);
            print(format, len);
            format += len;
        }
    }

    va_end(parameters);
}
