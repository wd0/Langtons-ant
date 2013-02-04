#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

enum { R = 1000, C = 1000 };

void
end(int x)
{
    endwin();
    exit(0);
}

int
getcoord(Point p)
{
    return p.x*C + p.y;
}

int
get_tile_color(Point p, Plane plane)
{
    return plane[getcoord(p)];
}

void
flip(Point p, Plane plane)
{
    int coord = getcoord(p);
    plane[coord] = !plane[coord];
}

void
rotate_ant(Ant *ant, int direc)
{
    if (direc == LEFT) {
        if (ant->direc == UP)
            ant->direc = LEFT;
        else
            ant->direc--;
    } else if (direc == RIGHT) {
        if (ant->direc == LEFT)
            ant->direc = UP;
        else
            ant->direc++;
    }
}

void
advance(Ant *ant)
{
    switch (ant->direc) {
    case UP:
        ant->p.y++;
        break;
    case RIGHT:
        ant->p.x++;
        break;
    case DOWN:
        ant->p.y--;
        break;
    case LEFT:
        ant->p.x--;
        break;
    }
}

void
guard(Ant *ant, Plane plane)
{
    if (0) /* It has not been proven
              that all starting positions bridge. */
        exit(0); /* Pray you've not chosen one of them */
}

int
step(Ant *ant, Plane plane)
{
    int tile_color = get_tile_color(ant->p, plane);

    if (tile_color == TILE_WHITE)
        rotate_ant(ant, RIGHT);
    else if (tile_color == TILE_BLACK)
        rotate_ant(ant, LEFT);
    else
        printw("OH GOD WHAT COLOR IS THIS");

    flip(ant->p, plane);
    guard(ant, plane);
    advance(ant);

    return 1;
}

void
movep(Point p)
{
    move(p.y, p.x);
}

void
draw(Point p, Plane plane)
{
    movep(p);
    attrset(COLOR_PAIR(get_tile_color(p, plane)));
    printw("#");
    refresh();
}


int main(int argc, char **argv)
{
    Ant *ant = makeant(100, 40, UP);
    Plane plane;

    signal(SIGINT, end);

    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_BLACK);
    } else {
        warn("Couldn't start_color(). Check if your term has_colors().");
        return 1;
    }

    plane = makeplane(R, C);
    movep(ant->p);
    draw(ant->p, plane);

    while (step(ant, plane))
        draw(ant->p, plane);

    freeant(ant);
    freeplane(plane);
    endwin();

    return 0;
}
