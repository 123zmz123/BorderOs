#include "stdint.h"
#include "stdarg.h"
#include "uart.h"

static int read_string(char *buffer, int position, const char *string)
{
    int index = 0;
    for (index = 0; string[index] != '\0'; index++)
    {
        buffer[position++] = string[index];
    }
    return index;
}

static int hex_to_string(char *buffer, int position, uint64_t digits)
{
    char digits_buffer[25];
    char digits_map[16] = "0123456789ABCDEF";
    int size = 0;

    do
    {
        digits_buffer[size++] = digits_map[digits % 16];
        digits /= 16;
    } while (digits != 0);

    for (int i = size - 1; i >= 0; i--)
    {
        buffer[position++] = digits_buffer[i];
    }

    buffer[position++] = 'H';

    return size + 1;
}

static int udecimal_to_string(char *buffer, int position, uint64_t digits)
{
    char digits_map[10] = "0123456789";
    char digits_buffer[25];
    int size = 0;

    do
    {
        digits_buffer[size++] = digits_map[digits % 10];
        digits /= 10;
    } while (digits != 0);

    for (int i = size - 1; i >= 0; i--)
    {
        buffer[position++] = digits_buffer[i];
    }

    return size;
}

static int decimal_to_string(char *buffer, int position, int64_t digits)
{
    int size = 0;

    if (digits < 0)
    {
        digits = -digits;
        buffer[position++] = '-';
        size = 1;
    }

    size += udecimal_to_string(buffer, position, (uint64_t)digits);
    return size;
}

static void write_console(const char *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        write_char(buffer[i]);
    }
}

int printk(const char *format, ...)
{
    char buf[1024];
    int buf_idx = 0;
    int64_t num = 0;
    char *str = 0;
    va_list args;
    va_start(args, format);
    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] != '%')
        {
            buf[buf_idx++] = format[i];
        }
        else
        {
            switch (format[++i])
            {
            case 'x':
                num = va_arg(args, int64_t);
                buf_idx += hex_to_string(buf, buf_idx, (uint64_t)num);
                break;
            case 'u':
                num = va_arg(args, int64_t);
                buf_idx += udecimal_to_string(buf, buf_idx, (uint64_t)num);
                break;
            case 'd':
                num = va_arg(args, int64_t);
                buf_idx += decimal_to_string(buf, buf_idx, num);
                break;

            case 's':
                str = va_arg(args, char*);
                buf_idx += read_string(buf, buf_idx, str);
                break;

            default:
                buf[buf_idx++] = '%';
                i--;
            }
        }
    }
    write_console(buf, buf_idx);
    va_end(args);
    return buf_idx;
}