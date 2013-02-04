#include <stdio.h>
#include <stdlib.h>

void
die(char *e)
{
    perror(e);
    exit(1);
}

void
warn(char *w)
{
    fprintf(stderr, "%s", w);
}

void *
emalloc(int size)
{
    void *m = malloc(size);
    if (!m)
        die("malloc");
    return m;
}
