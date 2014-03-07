#include <stdarg.h>
#include "Text.h"

std::string FormatText(const char* format, ...)
{
    std::string text;
    text.resize(256);

    va_list args;
    va_start(args, format);
    vsnprintf_s(&text[0], 256, 255, format, args);
    va_end(args);

    return text;
}