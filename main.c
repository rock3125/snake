#include <stdlib.h>
#include <ncurses.h>  // requires apt install libncurses-dev
#include <unistd.h>

#define ESC 27              // escape key
#define MAX_SIZE 100        // maximum size of the snake
#define FRAME_RATE 50000    // frame-rate in micro seconds

void initGame();    // initial set up
int readKeys();     // fwd decl. for reading the keyboard from the terminal
void gameLogic();   // game logic - movement, growing, and food management
void drawGame();    // draw the items on screen

// game over flag
int gameOver = FALSE;

// terminal size
int width = 0;
int height = 0;

// player's data
int prevX;
int prevY;
int x[MAX_SIZE];
int y[MAX_SIZE];
int size = 1;       // initial snake size
int dir = 0;        // direction the player is going in

// food location and number
int numFood = 10;
int foodX[MAX_SIZE];
int foodY[MAX_SIZE];

// get a random number in [lower, upper]
int getRandom(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

int main(void) {

    srand(1234);   // pre-set random
    initscr();          // init display
    curs_set(0);    // disable cursor display
    noecho();       // no echo for getch()
    cbreak();

    // getch() should not block
    nodelay(stdscr, TRUE);
    // get the screen-size
    getmaxyx(stdscr, height, width);

    initGame();

    // game loop
    int key_exit = FALSE;
    while (!gameOver) {

        usleep(FRAME_RATE);    // slow down

        key_exit = readKeys();
        if (key_exit)
            break;

        gameLogic();
        drawGame();

        refresh();
    }

    if (key_exit == 0) {
        initscr();
        curs_set(0);
        mvprintw(2, 0, "Game Over\n\npress any key to exit");
        nodelay(stdscr, FALSE);
        getch();
    }
    endwin();

    return EXIT_SUCCESS;
}

// read the keyboard - player's input
int readKeys() {
    int ch = getch();
    switch (ch) {
        case ESC:
            return TRUE;
        case KEY_UP:
        case 'w':
        case 'W':
            if (size == 1 || dir != KEY_DOWN)
                dir = KEY_UP;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (size == 1 || dir != KEY_UP)
                dir = KEY_DOWN;
            break;
        case KEY_LEFT:
        case 'd':
        case 'D':
            if (size == 1 || dir != KEY_RIGHT)
                dir = KEY_LEFT;
            break;
        case KEY_RIGHT:
        case 'a':
        case 'A':
            if (size == 1 || dir != KEY_LEFT)
                dir = KEY_RIGHT;
            break;
        default:
            break;
    }
    return FALSE;
}

// initialize player location, food positions
void initGame() {
    // set up the player's start location in the center of the screen
    for (int i = 0; i < size; i++) {
        x[i] = width / 2;
        y[i] = height / 2;
    }
    prevX = x[0];
    prevY = y[0];

    // set up the food for the snake in random locations
    for (int i = 0; i < numFood; i++) {
        foodX[i] = getRandom(1, width - 1);
        foodY[i] = getRandom(1, height - 1);
    }
}


// perform game logic
void gameLogic() {
    int last = size - 1;
    prevX = x[0];
    prevY = y[0];
    switch (dir) {
        case KEY_UP:
            if (y[last] > 0)
                y[last] -= 1;
            else
                gameOver = TRUE;
            break;
        case KEY_DOWN:
            if (y[last] < height)
                y[last] += 1;
            else
                gameOver = TRUE;
            break;
        case KEY_LEFT:
            if (x[last] < width)
                x[last] += 1;
            else
                gameOver = TRUE;
            break;
        case KEY_RIGHT:
            if (x[last] > 0)
                x[last] -= 1;
            else
                gameOver = TRUE;
            break;
        default:
            break;
    }

    // move the snake along its length when moving
    if (dir != 0) {
        for (int i = 0; i < size - 1; i++) {
            y[i] = y[i + 1];
            x[i] = x[i + 1];
        }
    }

    // manage food and growth
    for (int i = 0; i < numFood; i++) {
        if (foodY[i] == y[last] && foodX[i] == x[last]) {
            y[size] = y[last];
            x[size] = x[last];
            if (size + 1 < MAX_SIZE)
                size += 1; // we get longer
            // re set food position
            foodY[i] = getRandom(1, height - 1);
            foodX[i] = getRandom(1, width - 1);
            break;
        }
    }

}

// draw the game items onscreen
void drawGame() {
    // draw the food
    for (int i = 0; i < numFood; i++) {
        mvaddch(foodY[i], foodX[i], 'F');
    }
    // if we're moving, clear the previous location and move the snake
    if (dir != 0) {
        mvaddch(prevY, prevX, ' ');
    }
    // draw the snake
    for (int i = 0; i < size; i++) {
        mvaddch(y[i], x[i], 'O');
    }
}

