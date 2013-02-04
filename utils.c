#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <stdarg.h>

void
die(char *e)
{
    perror(e);
    exit(1);
}

void
fail(char *e, ...)
{
    va_list ap;

    va_start(ap, e);
    vfprintf(stderr, e, ap);
    va_end(ap);

    endwin();
    exit(1);
}

void
warn(char *w, ...)
{
    va_list ap;

    va_start(ap, w);
    vfprintf(stderr, w, ap);
    va_end(ap);
}


void *
emalloc(int size)
{
    void *m = malloc(size);
    if (!m)
        die("malloc");
    return m;
}
