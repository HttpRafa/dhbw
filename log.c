#include "log.h"

#include <stdarg.h>
#include <stdio.h>

#define INFO "INFO:"
#define WARN "WARN:"
#define ERROR "ERROR:"
#define DEBUG "DEBUG:"

void info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "%s %s\n", INFO, fmt);
    vprintf(buffer, args);

    va_end(args);
}

void warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "%s %s\n", WARN, fmt);
    vprintf(buffer, args);

    va_end(args);
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "%s %s\n", ERROR, fmt);
    vprintf(buffer, args);

    va_end(args);
}

void debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "%s %s\n", DEBUG, fmt);
    vprintf(buffer, args);

    va_end(args);
}