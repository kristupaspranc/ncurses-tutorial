#include <ncurses.h>
#include <string>
#include <array>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

std::array<std::string, 5> choices= {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit"
};

int n_choices = sizeof(choices) / sizeof(std::string);

void print_menu(WINDOW *menu_win, int highlight){
    int x, y;
    x = 2;
    y = 2;
    box(menu_win, 0, 0);

    for (std::size_t i = 0; i < n_choices; ++i){
        if (highlight == i + 1){ /* Highlight the present choice */
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
        ++y;
    }
    wrefresh(menu_win);
}

int main(){
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak();
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
    refresh();
    print_menu(menu_win, highlight);

    while (TRUE){
        c = wgetch(menu_win);
        switch(c){
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    highlight++;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                mvprintw(24, 0, "Character pressed is = %3d. Hopefully it can be produced\n", c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0)
            break;
    }

    mvprintw(23, 0, "You chose choice %d with choice string %s\n",
            choice, choices[choice - 1].c_str());
    clrtoeol();
    refresh();
    endwin();

    return 0;
}
