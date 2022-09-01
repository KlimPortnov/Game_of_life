#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

void draw(int **matrix);
void draw_f(int **matrix);
int create_matrix(int **matrix);
int create_point(int **matrix, int n, int m);
int change_matrix(int **matrix, int **new);
int init(int ***matrix);
void input(int **matrix);
void speed(char a, int *timesleep);

int main() {
    int timesleep = 1e6;
    int **matrix;
    int f = init(&matrix);
    if (!f) {
        int game = 1;
        input(matrix);
        FILE *file = freopen("/dev/tty", "r", stdin);
        if (file) {;}
        initscr();
        noecho();
        nodelay(stdscr, true);
        usleep(150000);
        int end = 0;
        while (game) {
            char a;
            draw(matrix);
            if (!create_matrix(matrix)) {
                game = 0;
                end = 1;
            }
            a = getch();
            if (a == 27)
                game = 0;
            speed(a, &timesleep);
            refresh();
            clear();
        }
        nodelay(stdscr, false);
        endwin();
        if (end) {
            system("clear");
            draw_f(matrix);
            printf("\n\nEND OF CHANGE !\n\n");
        }
        file = NULL;
    } else {
        printf("Device cant malloc !");
    }
    free(matrix);
    return 0;
}

void draw(int **matrix) {
    int rows = HEIGHT;
    int cols = WIDTH;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 0)
                printw(".");
            if (matrix[i][j] == 1)
                printw("0");
        }
        printw("\n");
    }
}

void draw_f(int **matrix) {
    int rows = HEIGHT;
    int cols = WIDTH;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 0)
                printf(".");
            if (matrix[i][j] == 1)
                printf("*");
        }
        printf("\n");
    }
}

int init(int ***matrix) {
    int f = 1;
    int rows = HEIGHT;
    int cols = WIDTH;
    (*matrix) = malloc(rows * cols * sizeof(int) + rows * sizeof(int*));
    int* ptr = (int*)((*matrix)+ rows);
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = ptr + cols * i;
    }
    if ((*matrix) != NULL)
        f = 0;
    return f;
}

int create_matrix(int **matrix) {
    int f = 1;
    int rows = HEIGHT;
    int cols = WIDTH;
    int **new = malloc(rows * cols * sizeof(int) + rows * sizeof(int*));
    int* ptr = (int*)(new + rows);
    for (int i = 0; i < rows; i++) {
        new[i] = ptr + cols * i;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            new[i][j] = create_point(matrix, i, j);
        }
    }
    f  = change_matrix(matrix, new);
    free(new);
    return f;
}

int create_point(int **matrix, int n, int m) {
    int sum = 0;
    int rows = HEIGHT;
    int cols = WIDTH;
    for (int i = n - 1; i < n + 2; i++) {
        for (int j = m - 1; j < m + 2; j++) {
            if ((i != n) || (j != m)) {
                sum += matrix[(i + rows) % rows][(j + cols) % cols];
            }
        }
    }
    if ((sum == 3) || ((sum == 2) && (matrix[n][m] == 1))) {
        sum = 1;
    } else {
        sum = 0;
    }
    return sum;
}

int  change_matrix(int **matrix, int **new) {
    int f = 0;
    int rows = HEIGHT;
    int cols = WIDTH;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] != new[i][j]) {
                f = 1;
            }
            matrix[i][j] = new[i][j];
        }
    }
    return f;
}

void input(int **matrix) {
    int a;
    int rows = HEIGHT;
    int cols = WIDTH + 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            a = getchar();
            if (a == 0 || a == 10) continue;
            matrix[i][j] = a - 48;
        }
    }
}

void speed(char a, int *timesleep) {
    if ((a == 43) && (*timesleep > 6e4))
        *timesleep -= 5e4;
    if ((a == 45) && (*timesleep < 1e8))
        *timesleep += 5e4;
    usleep(*timesleep);
}
