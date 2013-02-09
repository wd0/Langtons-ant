#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "types.h"
#include "utils.h"

#define LEN(x) (sizeof(x) / sizeof *x)

/* Rows and columns in the terminal upon starting. */
static int R, C;

static void
end(int x)
{
    endwin();
    exit(0);
}

static int
getcoord(Point p)
{
    return p.x*C + p.y;
}

static int
get_tile_color(Point p, Plane plane)
{
    return plane[getcoord(p)];
}

/* Toggle the tile color at p. */
static void
flip(Point p, Plane plane)
{
    int coord = getcoord(p);
    plane[coord] = !plane[coord];
}

static void
rotate_ant(Ant *ant, Direction direc)
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

static void
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

/* Protect the border. */
static void
guard(Ant *ant)
{
    /* if (ant->p.x >= R || ant->p.x < 0 || ant->p.y >= C || ant->p.y < 0) {
        getch();
        end(0);
    } */
    if (ant->p.x >= R)
        ant->p.x = 0;
    else if (ant->p.x < 0)
        ant->p.x = R - 1;

    if (ant->p.y >= C)
        ant->p.y = 0;
    else if (ant->p.y < 0)
        ant->p.y = C - 1;
}

static void
movep(Point p)
{
    move(p.y, p.x);
}

static void
draw_true_color(Point p, Plane plane)
{
    movep(p);
    attrset(COLOR_PAIR(get_tile_color(p, plane)));
    printw("#");
}

static void
draw(Point p, Plane plane)
{
    movep(p);
    attrset(COLOR_PAIR(2));
    printw("#");
}

static void
step(Ant *ant, Plane plane)
{
    int tile_color = get_tile_color(ant->p, plane);

    if (tile_color == TILE_WHITE)
        rotate_ant(ant, RIGHT);
    else if (tile_color == TILE_BLACK)
        rotate_ant(ant, LEFT);
    else
        printw("OH GOD WHAT COLOR IS THIS");

    draw_true_color(ant->p, plane);
    flip(ant->p, plane);
    advance(ant);
    guard(ant);
    draw(ant->p, plane);
    refresh();
}

/*
static void
step_ants(Ant *ants[], Plane plane, int n_ants)
{
    int i;

    for (i = 0; i < n_ants; ++i)
        step(ants[i], plane);
}
*/

void
init_colors(void)
{
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
    } else {
        fail("Couldn't start_color(). Check if your term has_colors().\n");
    }
}

void
init(void)
{
    initscr();
    signal(SIGINT, end);
    getmaxyx(stdscr, C, R);
    init_colors();
}

int main(int argc, char **argv)
{
    Ant *ant;
    Plane plane;

    init();

    if (argc == 3) {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);

        if (x < 0 || x >= R || y < 0 || y >= C)
            fail("Term size (%d, %d) inadequate to start at `(%d, %d)'.\n", R, C, x, y);
        ant = makeant(x, y, UP);
    } else {
        ant = makeant(R/2, C/2, UP);
    }

    plane = makeplane(R, C);

    for (;;)
        step(ant, plane);

    freeant(ant);
    freeplane(plane);
    endwin();
    return 0;
}
