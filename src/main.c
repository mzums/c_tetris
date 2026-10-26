#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

static long ms_now(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

struct Block
{
    bool active;
    int x;
    int y;
    int width;
    int height;
    const bool (*shape)[3];
};

struct Board
{
    bool active;
    int color;
    bool occupied;
};

#define HEIGHT 30
#define WIDTH 10
#define SHAPE_N 5

#define FRAME_MS 16
#define FALL_MS 1000

const bool SHAPES[SHAPE_N][3][3] = {
    {{0, 1, 0},
     {1, 1, 1},
     {0, 0, 0}},

    {{0, 1, 0},
     {0, 1, 0},
     {0, 1, 1}},

    {{0, 0, 1},
     {0, 0, 1},
     {0, 1, 1}},

    {{0, 1, 1},
     {1, 1, 0},
     {0, 0, 0}},

    {{1, 1, 0},
     {0, 1, 1},
     {0, 0, 0}},
};

bool check(const bool (*shape)[3], struct Board (*BOARD)[WIDTH], int x, int y)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (shape[i][j] && (y+i <= 0 || y+i>=20 || x+j<0 || x+j>=WIDTH || BOARD[y + i][x + j].occupied))
            {
                return false;
            }
        }
    }
    return true;
}

int main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    srand(time(NULL));

    struct Block b1;
    b1.x = 2;
    b1.y = 0;

    struct Board BOARD[HEIGHT][WIDTH];

    for (int i = 0; i < WIDTH; ++i)
    {
        BOARD[20][i].occupied = true;
    }

    printw("\033[2J\033[H");
    fflush(stdout);

    int r = rand() % SHAPE_N;
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

    int ch;
    long last_fall = ms_now();

    while (1)
    {
        ch = getch();
        erase();
        curs_set(0);
        move(0, 0);

        if (ch == 'q')
        {
            break;
        }
        if (ch == 27)
        {
            break;
        }

        napms(FRAME_MS);

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                if (BOARD[y][x].occupied)
                    printw(" @");
                else
                    printw(" .");
            }
            printw("\n");
        }

        refresh();
        move(0, 0);
        napms(16);

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

        long now = ms_now();
        if (b1.active && check(b1.shape, BOARD, b1.x, b1.y + 1) && now - last_fall >= FALL_MS)
        {
            last_fall = now;
            b1.y += 1;
        }
        else if (b1.active && !check(b1.shape, BOARD, b1.x, b1.y + 1))
        {
            b1.active = false;
        }

        if (ch == KEY_DOWN && b1.active && check(b1.shape, BOARD, b1.x, b1.y + 1))
        {
            b1.y++;
        }
        if (ch == KEY_RIGHT && b1.active && check(b1.shape, BOARD, b1.x+1, b1.y))
        {
            b1.x++;
        }
        if (ch == KEY_LEFT && b1.active && check(b1.shape, BOARD, b1.x-1, b1.y))
        {
            b1.x--;
        }

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
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