#include <ncurses.h>
#include <array>
#include <string>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

std::array<std::string, 5> choices{
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit"
};

int n_choices = sizeof(choices) / sizeof(std::string);

void print_menu(WINDOW *menu_win, int highlight);
void report_choice(int mouse_x, int mouse_y, int *p_choice);

int main(){
    int c, choice = 0;
    WINDOW *menu_win;
    MEVENT event;

    initscr();
    clear();
    noecho();
    cbreak();

    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    print_menu(menu_win, 1);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    while(TRUE){
        c = wgetch(menu_win);
        switch(c){
            case KEY_MOUSE:
                if (getmouse(&event) == OK){
                    if (event.bstate & BUTTON1_PRESSED){
                        report_choice(event.x + 1, event.y + 1, &choice);
                        if (choice == -1)
                            goto end;
                        mvprintw(22, 1, "Choice made is %d String chosen is %s",
                                choice, choices[choice - 1].c_str());
                        refresh();
                    }
                }
                print_menu(menu_win, choice);
                break;
        }
    }

end:
    endwin();
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight){
    int x = 2;
    int y = 2;
    box(menu_win, 0, 0);

    for (std::size_t i = 0; i < n_choices; ++i){
        if (highlight == i + 1){
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

void report_choice(int mouse_x, int mouse_y, int *p_choice){
    int i, j, choice;

    i = startx + 2;
    j = starty + 3;

    for (choice = 0; choice < n_choices; ++choice){
        if (mouse_y == j + choice && mouse_x >= i &&
                mouse_x <= i + choices[choice - 1].size()){
            if (choice == n_choices - 1)
                *p_choice = -1;
            else
                *p_choice = choice + 1;
            break;
        }
    }
}



