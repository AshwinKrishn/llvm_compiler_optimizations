#include <stdarg.h>
#include <stdio.h>

void my_printf(char *format, ...) {
    va_list argp;
    va_start(argp, format);
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == '%') {
                putchar('%');
            } else if (*format == 'c') {
                char char_to_print = va_arg(argp, int);
                putchar(char_to_print);
            } else {
                fputs("Not implemented", stdout);
            }
        } else {
            putchar(*format);
        }
        format++;
    }
    va_end(argp);
}

