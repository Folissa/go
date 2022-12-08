#include "go.h"


int main() {
    init_ncurses();


    WINDOW *legend_window = newwin(HEIGHT, WIDTH, 0, WINDOW_LEGEND_X);
    WINDOW *board_window = newwin(HEIGHT, WIDTH, 0, WINDOW_BOARD_X);

    PLAYER *black;
    black = create_player(BLACK);
    PLAYER *white;
    white = create_player(WHITE);

    FILE *legend_const;


    char *board_array[BOARD_SIZE][BOARD_SIZE_X];
    char *board_array_template[BOARD_SIZE][BOARD_SIZE_X];
    char *input_print = "";

    int logical_board_array[BOARD_SIZE][BOARD_SIZE];
    int legend_y = LEGEND_START_Y;
    int cursor_y, cursor_x;
    int input;
    int current_player;


    set_colors(legend_window, board_window);
    legend_const = fopen("legend_const.txt", "r");
    if (legend_const == NULL) {
        endwin();
        return 1;
    }
    create_board(board_array_template);
    new_game(legend_window, board_window, legend_const, black, white, &legend_y, &cursor_y, &cursor_x, &input_print, &current_player, logical_board_array, board_array);
    fclose(legend_const);
    do {
        print_legend_var(legend_window, black, white, legend_y, input_print, current_player, cursor_y, cursor_x);
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
                print_legend_var(legend_window, black, white, legend_y, input_print, current_player, cursor_y, cursor_x);
                int exit = 0;
                while(exit != 1) {
                    input = getch();
                    switch (input) {
                        case '\n':
                            exit = place_stone(board_window, black, white, &current_player, cursor_y, cursor_x, logical_board_array, board_array, board_array_template);
                            if (exit == 1) {
                                input_print = "enter";
                                print_legend_var(legend_window, black, white, legend_y, input_print, current_player, cursor_y, cursor_x);
                                place_stone(board_window, black, white, &current_player, cursor_y, cursor_x, logical_board_array, board_array, board_array_template);
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
                            break;
                    }
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
                new_game(legend_window, board_window, legend_const, black, white, &legend_y, &cursor_y, &cursor_x, &input_print, &current_player, logical_board_array, board_array);
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
    free(black);
    free(white);
    return 0;
}


void init_ncurses() {
    setlocale(LC_ALL, ""); // Enables unicode characters
    initscr(); // Initialises the curses mode (clears the screen) and allocates memory for stdscr
    raw(); // Control characters (like CTRL-C) are passed to the program without generating a signal
    noecho(); // Disables echoing of the input
    keypad(stdscr, TRUE); // Enables function keys and arrow keys
    curs_set(0); // Hides the cursor
}


void set_colors(WINDOW *legend_window, WINDOW *board_window) {
    start_color();
    init_color(100, 242 * 255 / 100, 176 * 255 / 100, 109 * 255 / 100); // Board background color (light brown).
    init_color(101, 65 * 255 / 100, 65 * 255 / 100, 65 * 255 / 100); // Font color (dark grey).
    init_color(102, 144 * 255 / 100, 168 * 255 / 100, 98 * 255 / 100); // Background color (green).
    init_pair(1, 101, COLOR_WHITE);
    init_pair(2, 101, 100);
    init_pair(3, 101, 102);
    wbkgd(stdscr, COLOR_PAIR(3));
    wbkgd(legend_window, COLOR_PAIR(3));
    wbkgd(board_window, COLOR_PAIR(3));
}


PLAYER *create_player(int player_color) {
    PLAYER  *new_player = (PLAYER *) malloc(sizeof(PLAYER));
    new_player->color = player_color;
    new_player->score = 0;
    return new_player;
}


void new_game(WINDOW *legend_window, WINDOW *board_window, FILE *legend_const, PLAYER *black, PLAYER *white, int *legend_y, int *cursor_y, int *cursor_x, char **input_print, int *current_player, int logical_board_array[][BOARD_SIZE], char* board_array[][BOARD_SIZE_X]) {
    refresh();
    *legend_y = LEGEND_START_Y;
    *cursor_y = 0;
    *cursor_x = 0;
    black->score = 0;
    white->score = 0;
    *current_player = BLACK;
    create_logical_board(logical_board_array);
    print_legend_const(legend_window, legend_y, legend_const);
    print_legend_var(legend_window, black, white, *legend_y, *input_print, *current_player, *cursor_y, *cursor_x);
    create_board(board_array);
    print_board(board_window, board_array);
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


void print_legend_var(WINDOW *window, PLAYER *black, PLAYER *white, int legend_y, char *input_print, int current_player, int cursor_y, int cursor_x) {
    wattron(window, A_BOLD);
    mvwprintw(window, legend_y, LEGEND_START_X, "    X: %d     ", cursor_x / 2 + 1);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    Y: %d     ", cursor_y + 1);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "Last key pressed: %s     ", input_print);
    legend_y += 1;
    if (current_player == BLACK) {
        mvwprintw(window, legend_y, LEGEND_START_X, "Now playing: Black     ");
    }
    else {
        mvwprintw(window, legend_y, LEGEND_START_X, "Now playing: White     ");
    }
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "Score: ");
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    Black: %d     ", black->score);
    legend_y += 1;
    mvwprintw(window, legend_y, LEGEND_START_X, "    White: %d     ", white->score);
    wattroff(window, A_BOLD);
    wrefresh(window);
}


void create_board(char *array[][BOARD_SIZE_X]) {
    for (int y = 0; y < BOARD_SIZE; y ++) {
        for (int x = 0; x < BOARD_SIZE_X; x ++) {
            if (x == 0 && y == 0) {
                array[y][x] = "\u250F";
            }
            else if ((x % 2 == 1 && y == 0) || (x % 2 == 1 && y == BOARD_SIZE - 1)) {
                array[y][x] = "\u2501";
            }
            else if (x % 2 == 1) {
                array[y][x] = "\u2500";
            }
            else if (y == 0 && x > 0 && x < BOARD_SIZE_X - 1) {
                array[y][x] = "\u252F";
            }
            else if (y == 0 && x == BOARD_SIZE_X - 1) {
                array[y][x] = "\u2513";
            }
            else if (y > 0 && y < BOARD_SIZE - 1 && x == 0) {
                array[y][x] = "\u2520";
            }
            else if (y == BOARD_SIZE - 1 && x > 0 && x < BOARD_SIZE_X - 1) {
                array[y][x] = "\u2537";
            }
            else if (y > 0 && y < BOARD_SIZE - 1 && x == BOARD_SIZE_X - 1) {
                array[y][x] = "\u2528";
            }
            else if (y == BOARD_SIZE - 1 && x == 0) {
                array[y][x] = "\u2517";
            }
            else if (y == BOARD_SIZE - 1 && x == BOARD_SIZE_X - 1) {
                array[y][x] = "\u251B";
            }
            else {
                array[y][x] = "\u253C";
            }
        }
    }
}


void create_logical_board(int logical_board_array[][BOARD_SIZE]) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            logical_board_array[y][x] = 0;
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


int suicide_check(int y, int x, int current_player, int logical_board_array[][BOARD_SIZE]) {
    if (((x - 1 >= 0 && logical_board_array[y][x - 1] == 0) || (x - 1 >= 0 && logical_board_array[y][x - 1] == current_player)) ||
    ((x + 1 < BOARD_SIZE && logical_board_array[y][x + 1] == 0) || (x + 1 < BOARD_SIZE && logical_board_array[y][x + 1] == current_player)) ||
    ((y + 1 < BOARD_SIZE && logical_board_array[y + 1][x] == 0) || (y + 1 < BOARD_SIZE && logical_board_array[y + 1][x] == current_player)) ||
    ((y - 1 >= 0 && logical_board_array[y - 1][x] == 0) || (y - 1 >= 0 && logical_board_array[y - 1][x] == current_player))) {
        return 0;
    }
    else {
        return 1;
    }
}


int place_stone(WINDOW *window, PLAYER *black, PLAYER *white, int *current_player, int cursor_y, int cursor_x, int logical_board_array[][BOARD_SIZE], char *board_array[][BOARD_SIZE_X], char *board_array_template[][BOARD_SIZE_X]) {
    char *stone_to_place;
    int y = cursor_y;
    int x = cursor_x / 2;
    if (logical_board_array[y][x] != 0) {
        return 0;
    }
    if (*current_player == BLACK) {
        if (capture(window, black, white, *current_player, y, x, logical_board_array, board_array, board_array_template) || !suicide_check(y, x, *current_player, logical_board_array)) {
            logical_board_array[y][x] = BLACK;
            stone_to_place = "\u26AB";
            *current_player = WHITE;
        }
        else {
            return 0;
        }
    }
    else if (*current_player == WHITE) {
        if (capture(window, black, white, *current_player, y, x, logical_board_array, board_array, board_array_template) || !suicide_check(y, x, *current_player, logical_board_array)) {
            logical_board_array[y][x] = WHITE;
            stone_to_place = "\u26AA";
            *current_player = BLACK;
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


int capture(WINDOW *window, PLAYER *black, PLAYER *white, int current_player, int y, int x, int logical_board_array[][BOARD_SIZE], char *board_array[][BOARD_SIZE_X], char *board_array_template[][BOARD_SIZE_X]) {
    int enemy;
    int captured = 0;
    int enemy_coordinates[4][2];
    int enemy_presence[4] = {0, 0, 0, 0};

    if (current_player == BLACK) {
        enemy = WHITE;
    }
    else if (current_player == WHITE) {
        enemy = BLACK;
    }

    for (int i = 0; i < 4; i ++ ) {
        for (int j = 0; i < 2; i ++ ) {
            enemy_coordinates[i][j] = 0;
        }
    }

    logical_board_array[y][x] = current_player;

    if (x - 1 >= 0 && logical_board_array[y][x - 1] == enemy) { // Check for the enemy around.
        enemy_coordinates[0][0] = y;
        enemy_coordinates[0][1] = x - 1;
        enemy_presence[0] = 1;

    }
    if (y - 1 >= 0 && logical_board_array[y - 1][x] == enemy) {
        enemy_coordinates[1][0] = y - 1;
        enemy_coordinates[1][1] = x;
        enemy_presence[1] = 1;
    }
    if (x + 1 < BOARD_SIZE && logical_board_array[y][x + 1] == enemy) {
        enemy_coordinates[2][0] = y;
        enemy_coordinates[2][1] = x + 1;
        enemy_presence[2] = 1;
    }
    if (y + 1 < BOARD_SIZE && logical_board_array[y + 1][x] == enemy) {
        enemy_coordinates[3][0] = y + 1;
        enemy_coordinates[3][1] = x;
        enemy_presence[3] = 1;
    }
    for (int i = 0; i < 4; i ++) {
        if (enemy_presence[i] == 1 && suicide_check(enemy_coordinates[i][0], enemy_coordinates[i][1], enemy, logical_board_array)) {
            if (current_player == BLACK) {
                black->score += 1;
            }
            else if (current_player == WHITE) {
                white->score += 1;
            }
            logical_board_array[enemy_coordinates[i][0]][enemy_coordinates[i][1]] = 0;
            board_array[enemy_coordinates[i][0]][enemy_coordinates[i][1] * 2] = board_array_template[enemy_coordinates[i][0]][enemy_coordinates[i][1] * 2];
            wattron(window, COLOR_PAIR(2));
            mvwprintw(window, BOARD_START_Y + enemy_coordinates[i][0], BOARD_START_X + enemy_coordinates[i][1] * 2, board_array[enemy_coordinates[i][0]][enemy_coordinates[i][1] * 2]);
            wattroff(window, COLOR_PAIR(2));
            if (BOARD_START_X + enemy_coordinates[i][1] * 2 + 1 < BOARD_START_X + BOARD_SIZE_X) {
                wattron(window, COLOR_PAIR(2));
                mvwprintw(window, BOARD_START_Y + enemy_coordinates[i][0], BOARD_START_X + enemy_coordinates[i][1] * 2 + 1, board_array[enemy_coordinates[i][0]][enemy_coordinates[i][1] * 2 + 1]);
                wattroff(window, COLOR_PAIR(2));
            }
            else {
                wattron(window, COLOR_PAIR(3));
                mvwprintw(window, BOARD_START_Y + enemy_coordinates[i][0], BOARD_START_X + enemy_coordinates[i][1] * 2 + 1, " ");
                wattroff(window, COLOR_PAIR(3));
            }
            wmove(window, y, x); // Move back to the coordinates of the inserted stone.
            wrefresh(window);
            captured = 1;
        }
    }
    if (captured) {
        return 1;
    }
    logical_board_array[y][x] = 0; // If capturing failed, we don't need this stone here.
    return 0;
}