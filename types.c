#include <stdlib.h>
#include "types.h"
#include "utils.h"

Plane
makeplane(int x, int y)
{
    int size = x * y;
    Plane plane = emalloc((size *= sizeof *plane));
    int i;

    for (i = 0; i < size; ++i)
        plane[i] = TILE_BLACK;

    return plane;
}

Ant *
makeant(int x, int y, Direction direc)
{
    Ant *ant = emalloc(sizeof *ant);

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    ant->p.x = x;
    ant->p.y = y;
    ant->direc = direc;

    return ant;
}

void
freeant(Ant *ant)
{
    if (ant)
        free(ant);
}

void
freeplane(Plane plane)
{
    if (plane)
        free(plane);
}
