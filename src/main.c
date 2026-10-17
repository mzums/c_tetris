#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct Block
{
    bool active;
    int x;
    int y;
    int width;
    int height;
};

struct Board
{
    bool active;
    int color;
};

const HEIGHT = 30;
const WIDTH = 10;

const SHAPES[7][4][4] = {
    {{1, 1, 1, 1},
     {0, 0, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    {{0, 1, 1, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    {{0, 1, 0, 0},
     {1, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    {{0, 1, 0, 0},
     {0, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0}},

    {{0, 0, 1, 0},
     {0, 0, 1, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0}},

    {{0, 1, 1, 0},
     {1, 1, 0, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},

    {{1, 1, 0, 0},
     {0, 1, 1, 0},
     {0, 0, 0, 0},
     {0, 0, 0, 0}},
};

main(void)
{
    srand(time(NULL));

    struct Block b1;
    b1.x = 2;
    b1.y = 0;

    int BOARD[HEIGHT][WIDTH];

    printf("\033[2J\033[H");
    fflush(stdout);

    int r = rand();

    while (1)
    {
        printf("\033[H");

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (x == b1.x && y == b1.y)
                    printf(" @");
                else
                    printf(" .");
            }
            printf("\n");
        }

        fflush(stdout);
        printf("\033[20A");
        usleep(1000000);
        if (b1.active)
        {
            b1.y += 1;
        }
    }

    return 0;
}