#ifndef GO_GO_H
#define GO_GO_H

#include <stdlib.h>
#include <locale.h>
#include <curses.h>
#include <string.h>

#define HEIGHT LINES // Height of the windows.
#define WIDTH ((COLS - 1) / 2)  // Width of the windows.

#define WINDOW_BOARD_X (COLS / 2 + 1) // Starting x coordinate for the window of the board.
#define BOARD_SIZE 19 // Size of the board.
#define BOARD_SIZE_X (BOARD_SIZE * 2 - 1) // Length of the board.
#define BOARD_START_Y ((HEIGHT - BOARD_SIZE) / 2) // Starting y coordinate for the board.
#define BOARD_START_X ((WIDTH - BOARD_SIZE_X) / 2) // Starting x coordinate for the board.

#define WINDOW_LEGEND_X 0 // Starting x coordinate for the window of the legend.
#define LEGEND_WIDTH 48  // The length of the longest string in the legend.
#define LEGEND_HEIGHT 18 // Number of lines of the legend.
#define LEGEND_START_Y ((HEIGHT - LEGEND_HEIGHT) / 2) // Starting y coordinate for the legend.
#define LEGEND_START_X ((WIDTH - LEGEND_WIDTH) / 2) // Starting x coordinate for the legend.

void print_legend_const(WINDOW *window, int *y, FILE *file); // Prints out the unchangeable part of the legend on the screen.
void print_legend_var(WINDOW *window, int y, char *input, int cursor_y, int cursor_x, int score_black, int score_white); // Prints out the changeable part of the legend on the screen.
void create_board(char *board_array[][BOARD_SIZE_X]); // Creates the board.
void print_board(WINDOW *window, char *board_array[][BOARD_SIZE_X]); // Prints out the board on the screen.
void move_cursor(WINDOW *window, char y_or_x, char action, int *y, int *x, char *board_array[][BOARD_SIZE_X]); // Moves the cursor and highlights its position.

#endif //GO_GO_H
