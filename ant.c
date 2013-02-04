#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

static int R, C;

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
guard(Ant *ant)
{
    if (ant->p.x >= R || ant->p.y >= C) {
        getch();
        end(0);
    }
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
    advance(ant);
    guard(ant);

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
    attrset(COLOR_PAIR(get_tile_color(p, plane) ));
    printw("#");
    refresh();
}


int main(int argc, char **argv)
{
    Ant *ant;
    Plane plane;

    initscr();
    signal(SIGINT, end);
    getmaxyx(stdscr, C, R);

    if (argc == 3) {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);

        if (x < 0 || x >= R || y < 0 || y >= C)
            fail("Term size (%d, %d) inadequate to start at `(%d, %d)'.\n", R, C, x, y);

        ant = makeant(x, y, UP);
    } else {
        ant = makeant(R/2, C/2, UP);
    }

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_BLACK);
    } else {
        fail("Couldn't start_color(). Check if your term has_colors().\n");
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
