#include <ncurses.h>
#include <string>
#include <array>

// Function to print the menu
void print_menu(WINDOW *menu_win, int highlight, const std::array<std::string, 5>& choices) {
    int x = 2, y = 2;
    box(menu_win, 0, 0);

    for (std::size_t i = 0; i < choices.size(); ++i) {
        if (highlight == i + 1) { // Highlight the current choice
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
        }
        ++y;
    }
    wrefresh(menu_win);
}

int main() {
    initscr(); // Initialize ncurses
    clear();
    noecho();
    cbreak(); // Disable line buffering
    keypad(stdscr, TRUE); // Enable function keys and arrow keys
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); // Enable all mouse events

    // Menu choices
    std::array<std::string, 5> choices = {
        "Choice 1",
        "Choice 2",
        "Choice 3",
        "Choice 4",
        "Exit"
    };

    int highlight = 1;
    int choice = 0;
    MEVENT event;

    // Create a window for the menu
    int startx = 0, starty = 0;
    int width = 20, height = 10;
    WINDOW *menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);

    // Print the initial menu
    print_menu(menu_win, highlight, choices);

    while (1) {
        int c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = choices.size();
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == choices.size())
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_PRESSED) {
                        int y = event.y - starty - 2;
                        if (y >= 0 && y < static_cast<int>(choices.size())) {
                            highlight = y + 1;
                            choice = highlight;
                        }
                    }
                }
                break;
            default:
                break;
        }
        print_menu(menu_win, highlight, choices);
        if (choice != 0) // User made a choice
            break;
    }

    mvprintw(height + 1, 0, "You chose choice %d with string %s", choice, choices[choice - 1].c_str());
    clrtoeol();
    refresh();
    getch();
    endwin(); // End ncurses mode

    return 0;
}
