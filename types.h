enum { TILE_BLACK, TILE_WHITE };
enum { UP, RIGHT, DOWN, LEFT };

typedef struct point Point;
struct point {
    int x;
    int y;
};

typedef char *Plane;

typedef struct ant Ant;
struct ant {
    Point p;
    int direc;
};

void *emalloc(int size);
Plane makeplane(int x, int y);
Ant *makeant(int x, int y, int direc);
void freeant(Ant *ant);
void freeplane(Plane plane);
