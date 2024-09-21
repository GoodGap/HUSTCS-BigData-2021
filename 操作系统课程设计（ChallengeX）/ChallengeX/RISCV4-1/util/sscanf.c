#include "util/sscanf.h"

#define F_SKIP   0001

#define S_DEFAULT   0
#define S_CONV      5

static int valid_sint(char c)
{
    if ((c >= '0' && c <= '9') || (c == '-' || c == '+'))
        return 0;
    return -1;
}

static int is_digit(char c)
{
    if (c >= '0' && c <= '9')
    	return 1;
    else
    	return 0;
}

static int is_space(char c)
{
    if (c == '\n' || c == '\r' || c == ' ' || c == '\f' || c == '\t' || c == '\v')
    	return 1;
    else
    	return 0;
}

static long long get_int(const char **str)
{
    long long n = 0;
    int temp = 0;
    char c;

    for (n = 0; **str; (*str)++)
    {
        c = **str;
        if (temp == 0 && c == '-')
        {
            temp = 1;
            continue;
        }
        if (!is_digit(c))
            if (c < 'A' || c > 'F')
                break;
        n = 10 * n + c - '0';
    }
    if (temp && n > 0)
        n = -n;

    return n;
}

static int get_str(const char **str, char *ptr, char *set, int width)
{
    int n, w, s;
    unsigned char c;
    if(width > 0)
    	w = 1;
    else
    	w = 0;
    if(ptr == NULL)
    	s = 1;
    else
    	s = 0;
    
    for (n = 0; **str; (*str)++, n++) {
        c = **str;
        if ((w && width-- == 0) || (!set && is_space(c)) || (set && (set[c] == 0)))
            break;
        if (s == 0)
            *ptr++ = c;
    }
    if (s == 0)
        *ptr = 0;
    return n;
}

int vsscanf(const char *str, const char *fmt, va_list ap)
{
    size_t n = 0;
    char s = S_DEFAULT;
    void *ptr;
    int base, sign, flags, width = 0, lflags;

    if (!fmt)
        return 0;

    for (; *fmt && *str; fmt++)
    {
        if (s == S_DEFAULT)
        {
            if (*fmt == '%')
            {
                flags = 0;
                s = S_CONV;
            }
            else if (is_space(*fmt))
                while (is_space(*str))
                    str++;
            else
                if (*fmt != *str++)
                    break;
            continue;
        }
        if (s == S_CONV)
        {
            if ('s' == *fmt)
            {
                s = S_DEFAULT;
                if (flags & F_SKIP)
                {
                    get_str(&str, NULL, NULL, width);
                    continue;
                }
                ptr = va_arg(ap, void *);
                get_str(&str, (char *)ptr, NULL, width);
                n++;
            }
            else if ('%' == *fmt)
            {
                s = S_DEFAULT;
                if (*str != '%')
                    break;
                str++;
            }
            else
                break;
        }
    }

    return n;
}
