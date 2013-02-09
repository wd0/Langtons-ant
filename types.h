enum { TILE_BLACK, TILE_WHITE };

typedef enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
} Direction;

typedef struct point Point;
struct point {
    int x;
    int y;
};

typedef char *Plane;

typedef struct ant Ant;
struct ant {
    Point p;
    Direction direc;
};

void *emalloc(int size);
Plane makeplane(int x, int y);
Ant *makeant(int x, int y, Direction direc);
void freeant(Ant *ant);
void freeplane(Plane plane);
