#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_SIZE 3
#define LEFT_PADDLE_X 2
#define RIGHT_PADDLE_X 77
#define WIN_SCORE 21

int ReadCommand(void);

void PrintScreen(int left_score, int right_score, int left_paddle_y, int right_paddle_y, int ball_x,
                 int ball_y);
void PrintCell(int row, int col, int left_paddle_y, int right_paddle_y, int ball_x, int ball_y);

int MoveLeftPaddle(int command, int left_paddle_y);
int MoveRightPaddle(int command, int right_paddle_y);

int IsPaddleCell(int row, int paddle_y);
int IsWallCell(int ball_y, int ball_dy);

int IsWinner(int left_score, int right_score);
void gameover(int left_score, int right_score);

int main(void) {
    int left_score = 0;
    int right_score = 0;

    int left_paddle_y = HEIGHT / 2;
    int right_paddle_y = HEIGHT / 2;

    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int ball_dx = 1;
    int ball_dy = 1;

    int command;
    int next_x;
    int next_y;
    int status = 1;

    while (status) {
        PrintScreen(left_score, right_score, left_paddle_y, right_paddle_y, ball_x, ball_y);

        if (IsWinner(left_score, right_score)) {
            gameover(left_score, right_score);
            status = 0;
        } else {
            command = ReadCommand();

            if (command == -1) {
                status = 0;
            } else if (command > 0) {
                left_paddle_y = MoveLeftPaddle(command, left_paddle_y);
                right_paddle_y = MoveRightPaddle(command, right_paddle_y);

                // Ограничиваем выход за верхнюю границу
                if (left_paddle_y < 2) {
                    left_paddle_y = 2;
                }
                if (right_paddle_y < 2) {
                    right_paddle_y = 2;
                }

                // Ограничиваем выход за нижнюю границу
                if (left_paddle_y > HEIGHT - 3) {
                    left_paddle_y = HEIGHT - 3;
                }
                if (right_paddle_y > HEIGHT - 3) {
                    right_paddle_y = HEIGHT - 3;
                }

                // Проверка на столкновения и изменение направления движения
                ball_dy = IsWallCell(ball_y, ball_dy);
                next_x = ball_x + ball_dx;
                next_y = ball_y + ball_dy;

                if (next_x == LEFT_PADDLE_X && IsPaddleCell(next_y, left_paddle_y)) {
                    ball_dx = -ball_dx;
                } else if (next_x == RIGHT_PADDLE_X && IsPaddleCell(next_y, right_paddle_y)) {
                    ball_dx = -ball_dx;
                }

                // Обновление координат
                ball_x += ball_dx;
                ball_y += ball_dy;

                // Увеличение счетчика, если забит гол
                if (ball_x <= 0) {
                    right_score++;
                    ball_x = WIDTH / 2;
                    ball_y = HEIGHT / 2;
                    ball_dx = 1;

                } else if (ball_x >= WIDTH - 1) {
                    left_score++;
                    ball_x = WIDTH / 2;
                    ball_y = HEIGHT / 2;
                    ball_dx = -1;
                }
            }
        }
    }

    return 0;
}

// Считывание команд с клавиатуры
int ReadCommand(void) {
    char sym;
    if (scanf("%c", &sym) != 1) return -1;
    if (sym == '\n') return 5;

    char junk;
    while (scanf("%c", &junk) == 1 && junk != '\n');

    if (sym == 'a' || sym == 'A') return 1;
    if (sym == 'z' || sym == 'Z') return 2;
    if (sym == 'k' || sym == 'K') return 3;
    if (sym == 'm' || sym == 'M') return 4;
    if (sym == ' ') return 5;

    return 0;
}

// Изменение положения ракеток
int MoveLeftPaddle(int command, int left_paddle_y) {
    if (command == 1) {
        left_paddle_y -= 1;
    }
    if (command == 2) {
        left_paddle_y += 1;
    }
    return left_paddle_y;
}

int MoveRightPaddle(int command, int right_paddle_y) {
    if (command == 3) {
        right_paddle_y -= 1;
    }
    if (command == 4) {
        right_paddle_y += 1;
    }
    return right_paddle_y;
}

// Проверка на столкновение
int IsPaddleCell(int ball_y, int paddle_y) {
    return (ball_y >= paddle_y - PADDLE_SIZE / 2 && ball_y <= paddle_y + PADDLE_SIZE / 2);
}

int IsWallCell(int ball_y, int ball_dy) {
    if (ball_y + ball_dy <= 0 || ball_y + ball_dy >= HEIGHT - 1) {
        ball_dy = -ball_dy;
    }
    return ball_dy;
}

// Отрисовка одного символа
void PrintCell(int row, int col, int left_paddle_y, int right_paddle_y, int ball_x, int ball_y) {
    if (row == 0 || row == HEIGHT - 1) {
        printf("-");
    } else if (col == 0 || col == WIDTH - 1) {
        printf("|");
    } else if (row == ball_y && col == ball_x) {
        printf("o");
    } else if (col == LEFT_PADDLE_X && IsPaddleCell(row, left_paddle_y)) {
        printf("]");
    } else if (col == RIGHT_PADDLE_X && IsPaddleCell(row, right_paddle_y)) {
        printf("[");
    } else {
        printf(" ");
    }
}

// Отрисовка всего поля
void PrintScreen(int left_score, int right_score, int left_paddle_y, int right_paddle_y, int ball_x,
                 int ball_y) {
    printf("\033[H\033[2J\033[3J");
    int row;
    int col;
    printf("\nPING PONG");
    printf("\nPlayer 1: %d | Player 2: %d\n", left_score, right_score);
    printf("Controls: A/Z and K/M, ENTER to skip\n\n");

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            PrintCell(row, col, left_paddle_y, right_paddle_y, ball_x, ball_y);
        }

        printf("\n");
    }
}

// Проверка на то, появился ли победитель
int IsWinner(int left_score, int right_score) {
    return (left_score >= WIN_SCORE || right_score >= WIN_SCORE);
}

// Отрисовка завершения игры
void gameover(int left_score, int right_score) {
    printf("\033[H\033[2J\033[3J");

    printf("PING PONG\n");
    printf("\nPlayer 1: %d | Player 2: %d\n", left_score, right_score);
    if (left_score > right_score) {
        if (left_score - right_score >= 15) {
            printf("\nPlayer 1 - KING of PING PONG\n\n");
        } else {
            printf("\nPlayer 1 - WINNER\n\n");
        }
    } else {
        if (left_score - right_score <= -15) {
            printf("\nPlayer 2 - KING of PING PONG\n\n");
        } else {
            printf("\nPlayer 2 - WINNER\n\n");
        }
    }
}
