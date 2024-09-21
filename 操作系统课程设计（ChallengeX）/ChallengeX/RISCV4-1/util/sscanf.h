#ifndef _VSNPRINTF_H
#define _VSNPRINTF_H

#include <stdarg.h>

#include "util/string.h"
#include "util/types.h"

int vsscanf(const char *str, const char *fmt, va_list ap);

#endif
