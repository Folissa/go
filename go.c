#include "go.h"


// TODO: Set the window title
// TODO: Make it possible to move through the board with arrows (cursor can't move outside the board and should be visible)
// TODO: New game - n should reset the player tiles and return the board to the initial state
// TODO: Simple stone placing
// TODO: Simple capturing
// TODO: Comment constants


int main() {
    setlocale(LC_ALL, ""); // Enables unicode characters
    initscr(); // Initialises the curses mode (clears the screen) and allocates memory for stdscr
  /*  raw(); // Control characters (like CTRL-C) are passed to the program without generating a signal*/
    noecho(); // Disables echoing of the input
    keypad(stdscr, TRUE); // Enables function keys and arrow keys
    curs_set(0); // Hides the cursor
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);

    char *board_array[BOARD_SIZE][BOARD_SIZE_X];
    int legend_y = LEGEND_START_Y ;
    int cursor_y = 0, cursor_x = 0;
    int input;
    char *input_print = "";
    int score_black = 0, score_white = 0;

    WINDOW *legend_window = newwin(HEIGHT, WIDTH, 0, WINDOW_LEGEND_X);
    FILE *legend_const;
    legend_const = fopen("legend_const.txt", "r");
    if (legend_const == NULL) {
        endwin();
        return 1;
    }
    print_legend_const(legend_window, &legend_y, legend_const);
    fclose(legend_const);

    WINDOW *board_window = newwin(HEIGHT, WIDTH, 0, WINDOW_BOARD_X);
    create_board(board_array);
    print_board(board_window, board_array);

    do {
        print_legend_var(legend_window, legend_y, input_print, cursor_y, cursor_x, score_black, score_white);
        input = getch();
        switch (input) {
            case KEY_UP:
                input_print = "up";
                if (0 <= cursor_y - 1 && cursor_y - 1 < BOARD_SIZE ) {
                    move_cursor(board_window, 'y', 's', &cursor_y, &cursor_x, board_array);
                }
                break;
            case KEY_RIGHT:
                input_print = "right";
                if (0 <= cursor_x + 1 && cursor_x + 1 < BOARD_SIZE_X ) {
                    move_cursor(board_window, 'x', 'a', &cursor_y, &cursor_x, board_array);
                }
                break;
            case KEY_DOWN:
                input_print = "down";
                if (0 <= cursor_y + 1 && cursor_y + 1 < BOARD_SIZE ) {
                    move_cursor(board_window, 'y', 'a', &cursor_y, &cursor_x, board_array);
                }
                break;
            case KEY_LEFT:
                input_print = "left";
                if (0 <= cursor_x - 1 && cursor_x - 1 < BOARD_SIZE_X ) {
                    move_cursor(board_window, 'x', 's', &cursor_y, &cursor_x, board_array);
                }
                break;
            case 'i':
                input_print = "i";
                print_legend_var(legend_window, legend_y, input_print, cursor_y, cursor_x, score_black, score_white);
                int exit = 0;
                while(exit != 1) {
                    input = getch();
                    switch (input) {
                        case '\n':
                            // Place a stone
                            wattron(board_window, COLOR_PAIR(2));
                            mvwprintw(board_window, BOARD_START_Y + cursor_y, BOARD_START_X + cursor_x, "\u25CF");
                            wattroff(board_window, COLOR_PAIR(2));
                            board_array[cursor_y][cursor_x] = "\u25CF";
                            wrefresh(board_window);
                            exit = 1;
                            break;
                        case 'c':
                            exit = 1;
                            break;
                    }
                }
                break;
            case 's':
                input_print = "s";
                break;
            case 'n':
                input_print = "n";
                break;
            case 'l':
                input_print = "l";
                break;
            case 'f':
                input_print = "f";
                break;
            default:
                break;
        }
    } while (input != 'q');

    endwin(); // Ends the curses mode
    return 0;
}


/*
struct player {
    enum {
        BLACK,
        WHITE
    };
    int color;
    int score;
} black, white;
*/


void print_legend_const(WINDOW *window, int *y, FILE *file) {
    char line[50];
    while (fgets(line, sizeof line, file) != NULL) {
        mvwprintw(window, *y, LEGEND_START_X, "%s", line); // Write text to the stdscr buffer
        *y += 1;
    }
    box(window, 0, 0);
    refresh(); // Dumps the contents from the stdscr buffer on the screen
    wrefresh(window);
}


void print_legend_var(WINDOW *window, int y, char *input, int cursor_y, int cursor_x, int score_black, int score_white) {
    mvwprintw(window, y, LEGEND_START_X, "    X: %d     ", cursor_x / 2 + 1);
    y += 1;
    mvwprintw(window, y, LEGEND_START_X, "    Y: %d     ", cursor_y + 1);
    y += 1;
    mvwprintw(window, y, LEGEND_START_X, "Last key pressed: %s     ", input);
    y += 1;
    mvwprintw(window, y, LEGEND_START_X, "Score: ");
    y += 1;
    mvwprintw(window, y, LEGEND_START_X, "    Black: %d     ", score_black);
    y += 1;
    mvwprintw(window, y, LEGEND_START_X, "    White: %d     ", score_white);
    wrefresh(window);
}


void create_board(char *board_array[][BOARD_SIZE_X]) {
    for (int y = 0; y < BOARD_SIZE; y ++) {
        for (int x = 0; x < BOARD_SIZE_X; x ++) {
            if (x == 0 && y == 0) {
                board_array[y][x] = "\u250F";
            }
            else if ((x % 2 == 1 && y == 0) || (x % 2 == 1 && y == BOARD_SIZE - 1)) {
                board_array[y][x] = "\u2501";
            }
            else if (x % 2 == 1) {
                board_array[y][x] = "\u2500";
            }
            else if (y == 0 && x > 0 && x < BOARD_SIZE_X - 1) {
                board_array[y][x] = "\u252F";
            }
            else if (y == 0 && x == BOARD_SIZE_X - 1) {
                board_array[y][x] = "\u2513";
            }
            else if (y > 0 && y < BOARD_SIZE - 1 && x == 0) {
                board_array[y][x] = "\u2520";
            }
            else if (y == BOARD_SIZE - 1 && x > 0 && x < BOARD_SIZE_X - 1) {
                board_array[y][x] = "\u2537";
            }
            else if (y > 0 && y < BOARD_SIZE - 1 && x == BOARD_SIZE_X - 1) {
                board_array[y][x] = "\u2528";
            }
            else if (y == BOARD_SIZE - 1 && x == 0) {
                board_array[y][x] = "\u2517";
            }
            else if (y == BOARD_SIZE - 1 && x == BOARD_SIZE_X - 1) {
                board_array[y][x] = "\u251B";
            }
            else {
                board_array[y][x] = "\u253C";
            }
        }
    }
}


void print_board(WINDOW *window, char *board_array[][BOARD_SIZE_X]) {
    for (int y = 0; y < BOARD_SIZE; y ++) {
        for (int x = 0; x < BOARD_SIZE_X; x++) {
            if (x == 0 && y == 0) {
                wattron(window, COLOR_PAIR(2));
                mvwprintw(window, y + BOARD_START_Y, x + BOARD_START_X, board_array[y][x]);
                wattroff(window, COLOR_PAIR(2));
            }
            else {
                mvwprintw(window, y + BOARD_START_Y, x + BOARD_START_X, board_array[y][x]);
            }
        }
    }
    box(window, 0, 0);
    wrefresh(window);
}


void move_cursor(WINDOW *window, char y_or_x, char action, int *y, int *x, char *board_array[][BOARD_SIZE_X]) {
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, BOARD_START_Y + *y, BOARD_START_X + *x, "%s", board_array[*y][*x]);
    wattroff(window, COLOR_PAIR(1));
    if (y_or_x == 'y' && action == 'a') {
        *y += 1;
    }
    else if (y_or_x == 'y' && action == 's') {
        *y -= 1;
    }
    else if (y_or_x == 'x' && action == 'a') {
        *x += 2;
    }
    else if (y_or_x == 'x' && action == 's') {
        *x -= 2;
    }
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, BOARD_START_Y + *y, BOARD_START_X + *x, "%s", board_array[*y][*x]);
    wattroff(window, COLOR_PAIR(2));
    wrefresh(window);
}