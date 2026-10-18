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
    const bool (*shape)[4];
};

struct Board
{
    bool active;
    int color;
    bool occupied;
};

#define HEIGHT 30
#define WIDTH 10

const bool SHAPES[7][4][4] = {
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

bool check(const bool (*shape)[4], struct Board (*BOARD)[WIDTH], int x, int y)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (shape[i][j] && BOARD[y + i][x + j].occupied)
            {
                return false;
            }
        }
    }
    return true;
}

int main(void)
{
    srand(time(NULL));

    struct Block b1;
    b1.x = 2;
    b1.y = 0;

    struct Board BOARD[HEIGHT][WIDTH];

    for (int i = 0; i < WIDTH; ++i)
    {
        BOARD[20][i].occupied = true;
    }

    printf("\033[2J\033[H");
    fflush(stdout);

    int r = rand() % 7;
    b1.shape = SHAPES[r];
    b1.active = true;

    for (int y = 0; y < HEIGHT; y++)
    {
        if (y == 20)
            continue;
        for (int x = 0; x < WIDTH; x++)
        {
            BOARD[y][x].occupied = false;
            BOARD[y][x].active = false;
        }
    }

    while (1)
    {
        printf("\033[H");

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (BOARD[y][x].occupied)
                    printf(" @");
                else
                    printf(" .");
            }
            printf("\n");
        }

        fflush(stdout);
        printf("\033[20A");
        usleep(1000000);

        for (int y = 0; y < HEIGHT; y++)
        {
            if (y == 20)
                continue;
            for (int x = 0; x < WIDTH; x++)
            {
                BOARD[y][x].occupied = false;
                BOARD[y][x].active = false;
            }
            printf("\n");
        }

        if (b1.active && check(b1.shape, BOARD, b1.x, b1.y + 1))
        {
            b1.y += 1;
        }

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (b1.shape[i][j] == 1)
                {
                    BOARD[b1.y + i][b1.x + j].active = 1;
                    BOARD[b1.y + i][b1.x + j].occupied = 1;
                }
            }
        }
    }

    return 0;
}