#include "go.h"


enum {
    BLACK = 1,
    WHITE = 2
};


typedef struct player {
    int color;
    int score;
} black, white;


int main() {
    init();

    WINDOW *legend_window = newwin(HEIGHT, WIDTH, 0, WINDOW_LEGEND_X);
    WINDOW *board_window = newwin(HEIGHT, WIDTH, 0, WINDOW_BOARD_X);
    FILE *legend_const;

    char *board_array[BOARD_SIZE][BOARD_SIZE_X];
    int logical_board_array[BOARD_SIZE][BOARD_SIZE];
    int legend_y = LEGEND_START_Y;
    int cursor_y, cursor_x;
    int input;
    int current_player;
    char *input_print = "";
    int score_black, score_white;

    legend_const = fopen("legend_const.txt", "r");
    if (legend_const == NULL) {
        endwin();
        return 1;
    }

    wbkgd(legend_window, COLOR_PAIR(3));
    wbkgd(board_window, COLOR_PAIR(3));
    new_game(legend_window, board_window, legend_const, &legend_y, &cursor_y, &cursor_x, &input_print, &current_player, &score_black, &score_white, logical_board_array, board_array);
    fclose(legend_const);
    do {
        print_legend_var(legend_window, legend_y, input_print, current_player, cursor_y, cursor_x, score_black, score_white);
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
                print_legend_var(legend_window, legend_y, input_print, current_player, cursor_y, cursor_x, score_black, score_white);
                int exit = 0;
                while(exit != 1) {
                    input = getch();
                    switch (input) {
                        case '\n':
                            exit = place_stone(board_window, &current_player, cursor_y, cursor_x, logical_board_array, board_array);
                            if (exit == 1) {
                                input_print = "enter";
                                print_legend_var(legend_window, legend_y, input_print, current_player, cursor_y, cursor_x, score_black, score_white);
                                place_stone(board_window, &current_player, cursor_y, cursor_x, logical_board_array, board_array);
                                break;
                            }
                            else {
                                exit = 1;
                                break;
                            }
                        case 'c':
                            input_print = "c";
                            exit = 1;
                            break;
                        default:
                            break;                    }
                }
                break;
            case 's':
                input_print = "s";
                break;
            case 'n':
                input_print = "n";
                legend_const = fopen("legend_const.txt", "r");
                if (legend_const == NULL) {
                    endwin();
                    return 1;
                }
                new_game(legend_window, board_window, legend_const, &legend_y, &cursor_y, &cursor_x, &input_print, &current_player, &score_black, &score_white, logical_board_array, board_array);
                fclose(legend_const);
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


void print_legend_const(WINDOW *window, int *legend_y, FILE *file) {
    char line[50];
    wattron(window, A_BOLD);
    while (fgets(line, sizeof line, file) != NULL) {
        mvwprintw(window, *legend_y, LEGEND_START_X, "%s", line); // Write text to the stdscr buffer
        *legend_y += 1;
    }
    wattroff(window, A_BOLD);
    box(window, 0, 0);
    wrefresh(window);
}


void print_legend_var(WINDOW *window, int legend_y, char *input_print, int current_player, int cursor_y, int cursor_x, int score_black, int score_white) {
    wattron(window, A_BOLD);
    mvwprintw(window, legend_y, LEGEND_START_X, "    X: %d     ", cursor_x / 2 + 1);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    Y: %d     ", cursor_y + 1);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "Last key pressed: %s     ", input_print);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "Score: ");
    legend_y += 1;
    if (current_player == BLACK) {
        mvwprintw(window, legend_y, LEGEND_START_X, "Now playing: Black     ");
    }
    else {
        mvwprintw(window, legend_y, LEGEND_START_X, "Now playing: White     ");
    }
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    Black: %d     ", score_black);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    White: %d     ", score_white);
    wattroff(window, A_BOLD);
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
                wattron(window, COLOR_PAIR(1));
                mvwprintw(window, y + BOARD_START_Y, x + BOARD_START_X, board_array[y][x]);
                wattroff(window, COLOR_PAIR(1));
            }
            else {
                wattron(window, COLOR_PAIR(2));
                mvwprintw(window, y + BOARD_START_Y, x + BOARD_START_X, board_array[y][x]);
                wattroff(window, COLOR_PAIR(2));
            }
        }
    }
    box(window, 0, 0);
    wrefresh(window);
}


void create_logical_board(int logical_board_array[][BOARD_SIZE]) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            logical_board_array[y][x] = 0;
        }
    }
}


void move_cursor(WINDOW *window, char y_or_x, char action, int *y, int *x, char *board_array[][BOARD_SIZE_X]) {
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, BOARD_START_Y + *y, BOARD_START_X + *x, "%s", board_array[*y][*x]);
    wattroff(window, COLOR_PAIR(2));
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
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, BOARD_START_Y + *y, BOARD_START_X + *x, "%s", board_array[*y][*x]);
    wattroff(window, COLOR_PAIR(1));
    wrefresh(window);
}

int place_stone(WINDOW *window, int *current_player, int cursor_y, int cursor_x, int logical_board_array[][BOARD_SIZE], char *board_array[][BOARD_SIZE_X]) {
    char *stone_to_place;
    int y = cursor_y;
    int x = cursor_x / 2;
    if (*current_player == BLACK) {
        if (!suicide_check(y, x, *current_player, logical_board_array)) {
            logical_board_array[y][x] = BLACK;
            stone_to_place = "\u26AB";
            *current_player += 1;
        }
        else {
            return 0;
        }
    }
    else if (*current_player == WHITE) {
        if (!suicide_check(y, x, *current_player, logical_board_array)) {
            logical_board_array[y][x] = WHITE;
            stone_to_place = "\u26AA";
            *current_player -= 1;
        }
        else {
            return 0;
        }
    }
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, BOARD_START_Y + cursor_y, BOARD_START_X + cursor_x, stone_to_place);
    wattroff(window, COLOR_PAIR(1));
    board_array[cursor_y][cursor_x] = stone_to_place;
    wrefresh(window);
    return 1;
}

void new_game(WINDOW *legend_window, WINDOW *board_window, FILE *legend_const, int *legend_y, int *cursor_y, int *cursor_x, char **input_print, int *current_player, int *score_black, int *score_white, int logical_board_array[][BOARD_SIZE], char* board_array[][BOARD_SIZE_X]) {
    refresh();
    *legend_y = LEGEND_START_Y;
    *cursor_y = 0;
    *cursor_x = 0;
    *current_player = BLACK;
    *score_black = 0;
    *score_white = 0;
    create_logical_board(logical_board_array);
    print_legend_const(legend_window, legend_y, legend_const);
    print_legend_var(legend_window, *legend_y, *input_print, *current_player, *cursor_y, *cursor_x, *score_black, *score_white);
    create_board(board_array);
    print_board(board_window, board_array);
}

void init() {
    setlocale(LC_ALL, ""); // Enables unicode characters
    initscr(); // Initialises the curses mode (clears the screen) and allocates memory for stdscr
/*
    raw(); // Control characters (like CTRL-C) are passed to the program without generating a signal
*/
    noecho(); // Disables echoing of the input
    keypad(stdscr, TRUE); // Enables function keys and arrow keys
    curs_set(0); // Hides the cursor
    set_colors();
}

void set_colors() {
    start_color();
    init_color(100, 242 * 255 / 100, 176 * 255 / 100, 109 * 255 / 100); // Board background color.
    init_color(101, 65 * 255 / 100, 65 * 255 / 100, 65 * 255 / 100); // Font color.
    init_color(102, 144 * 255 / 100, 168 * 255 / 100, 98 * 255 / 100); // Background color.
    init_pair(1, 101, COLOR_WHITE);
    init_pair(2, 101, 100);
    init_pair(3, 101, 102);
    wbkgd(stdscr, COLOR_PAIR(3));
}

int suicide_check( int y, int x, int current_player, int logical_board_array[][BOARD_SIZE]) {
    if (logical_board_array[y][x] != 0) {
        return 1;
    }
    if ((x - 1 >= 0 && logical_board_array[y][x - 1] == 0) ||
        (x + 1 < BOARD_SIZE && logical_board_array[y][x + 1] == 0) ||
        (y + 1 >= 0 && logical_board_array[y + 1][x] == 0) ||
        (y - 1 < BOARD_SIZE && logical_board_array[y - 1][x] == 0)) {
        return 0;
    }
    else if ((x - 1 >= 0 && logical_board_array[y][x - 1] != current_player) &&
            (x + 1 < BOARD_SIZE && logical_board_array[y][x + 1] != current_player) &&
            (y + 1 >= 0 && logical_board_array[y + 1][x] != current_player) &&
            (y - 1 < BOARD_SIZE && logical_board_array[y - 1][x] != current_player)) {
        return 1;
    }
    else {
        return 0;
    }
}