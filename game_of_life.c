#include <ncurses.h>
#include <stdio.h>

#define HEIGHT 25
#define WIDTH 80
#define SPD_DL 30
#define MAX_SPD_DL 1000
#define MIN_SPD_DL 20
#define START_SPD 200

void Control(int *speed, int *status);
void PrintScreen(const int arr[HEIGHT][WIDTH]);
int CountNeighbors(const int arr[HEIGHT][WIDTH], int y, int x);
void MakeStep(int current[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);
void CopyField(int arr1[HEIGHT][WIDTH], int arr2[HEIGHT][WIDTH]);
void ReadFile(int arr[HEIGHT][WIDTH]);
void RuleOfLife(int current[HEIGHT][WIDTH], int next[HEIGHT][WIDTH], int y, int x);
void FillArr(int arr[HEIGHT][WIDTH], int i, int j);

int main(void) {
    int current_generation[HEIGHT][WIDTH];
    int next_generation[HEIGHT][WIDTH];

    ReadFile(current_generation);
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        printf("n/a");
        return 1;
    }

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    int speed = START_SPD;
    int status = 1;

    while (status) {
        clear();
        move(0, 0);

        PrintScreen(current_generation);
        refresh();

        Control(&speed, &status);

        MakeStep(current_generation, next_generation);
        CopyField(next_generation, current_generation);

        napms(speed);
    }
    endwin();
    return 0;
}

int CountNeighbors(const int arr[HEIGHT][WIDTH], int y, int x) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy != 0 || dx != 0) {
                int neighbor_y = (y + dy + HEIGHT) % HEIGHT;
                int neighbor_x = (x + dx + WIDTH) % WIDTH;
                if (arr[neighbor_y][neighbor_x] == 1) count++;
            }
        }
    }
    return count;
}

void MakeStep(int current[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            RuleOfLife(current, next, y, x);
        }
    }
}

void CopyField(int arr1[HEIGHT][WIDTH], int arr2[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            arr2[y][x] = arr1[y][x];
        }
    }
}
void Control(int *speed, int *status) {
    int sym;
    int last_sym = ERR;

    while ((sym = getch()) != ERR) {
        last_sym = sym;
    }

    if (last_sym == 'z' || last_sym == 'Z') {
        if (*speed < MAX_SPD_DL) {
            *speed += SPD_DL;
        }
    } else if (last_sym == 'a' || last_sym == 'A') {
        if (*speed > MIN_SPD_DL) {
            *speed -= SPD_DL;
        }
    } else if (last_sym == ' ') {
        *status = 0;
    }
}

void PrintScreen(const int arr[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] == 1)
                printw("o");
            else
                printw(" ");
        }
        printw("\n");
    }
}

void ReadFile(int arr[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            FillArr(arr, i, j);
        }
    }
}

void RuleOfLife(int current[HEIGHT][WIDTH], int next[HEIGHT][WIDTH], int y, int x) {
    int neighbors = CountNeighbors(current, y, x);
    if (current[y][x] == 1) {
        if (neighbors == 2 || neighbors == 3)
            next[y][x] = 1;
        else
            next[y][x] = 0;
    } else {
        if (neighbors == 3)
            next[y][x] = 1;
        else
            next[y][x] = 0;
    }
}

void FillArr(int arr[HEIGHT][WIDTH], int i, int j) {
    char ch;
    if (scanf(" %c", &ch) == 1) {
        if (ch == '1')
            arr[i][j] = 1;
        else
            arr[i][j] = 0;
    } else {
        arr[i][j] = 0;
    }
}