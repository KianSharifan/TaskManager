#include <ncurses.h>

void getting_priority(int *priority,WINDOW *win_tasks,int highlight_pointer);

void getting_priority(int *prioritys,WINDOW *win_tasks,int highlight_pointer)
{
int user_choice = 0;
while (user_choice != 10)
{
    user_choice = getch();
    if (user_choice == 49)
    {
        *prioritys = 1;
        mvwprintw(win_tasks,highlight_pointer+1,1,"1");
        wrefresh(win_tasks);
    }
    else if (user_choice == 50)
    {
        *prioritys = 2;
        mvwprintw(win_tasks,highlight_pointer+1,1,"2");
        wrefresh(win_tasks);
    }
    else if (user_choice == 51)
    {
        *prioritys = 3;
        mvwprintw(win_tasks,highlight_pointer+1,1,"3");
        wrefresh(win_tasks);
    }
    else if (user_choice == 52)
    {
        *prioritys = 4;
        mvwprintw(win_tasks,highlight_pointer+1,1,"4");
        wrefresh(win_tasks);
    }
    else if (user_choice == 53)
    {
        *prioritys = 5;
        mvwprintw(win_tasks,highlight_pointer+1,1,"5");
        wrefresh(win_tasks);
    }
    else if (user_choice == 54)
    {
        *prioritys = 6;
        mvwprintw(win_tasks,highlight_pointer+1,1,"6");
        wrefresh(win_tasks);
    }
    else if (user_choice == 55)
    {
        *prioritys = 7;
        mvwprintw(win_tasks,highlight_pointer+1,1,"7");
        wrefresh(win_tasks);
    }
    else if (user_choice == 56)
    {
        *prioritys = 8;
        mvwprintw(win_tasks,highlight_pointer+1,1,"8");
        wrefresh(win_tasks);
    }
    else if (user_choice == 57)
    {
        *prioritys = 9;
        mvwprintw(win_tasks,highlight_pointer+1,1,"9");
        wrefresh(win_tasks);
    }
    else if (user_choice == 48)
    {
        *prioritys = 0;
        mvwprintw(win_tasks,highlight_pointer+1,1,"0");
        wrefresh(win_tasks);
    }
    else
    {
        // :)
    }
}
}