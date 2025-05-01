#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include "getting_parameters.c"
#ifndef getting_parameters_H
#define getting_parameters_H
int getting_window_x(int x);
int getting_window_y(int y);
#endif
#include "valid_deadline.c"
#ifndef valid_deadline_H
#define valid_deadline_H
int is_valid(char *deadline);
#endif
#include "getting_priority.c"
#ifndef getting_priority_H
#define getting_priority_H
void getting_priority(int *priority,WINDOW *win_tasks,int highlight_pointer);
#endif
#include "sooner_deadline.c"
#ifndef sooner_deadline_H
#define sooner_deadline_H
int sooner_deadline(char *deadline1,char *deadline2);
#endif
#include "sooner_alphabetically.c"
#ifndef sooner_alphabetically_H
#define sooner_alphabetically_H
int sooner_alphabetically(char *name1,char *name2);
#endif

char *load_json_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = malloc(file_size + 1);
    if (!json_data) {
        printf("Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    fread(json_data, 1, file_size, file);
    json_data[file_size] = '\0';

    fclose(file);
    return json_data;
}

void save_json_to_file(const char *filename, const char *json_data) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing: %s\n", filename);
        return;
    }

    fprintf(file, "%s", json_data);

    fclose(file);
}


void options(int option,WINDOW *win_task,int y_max,int x_max,WINDOW *win_deadline,WINDOW *win_description,WINDOW *win_categories,WINDOW *win_subtasks);
void clear_box_content(WINDOW *win, int start_y, int start_x, int height, int width) {
    for (int y = start_y + 1; y < height - 1; y++) {
        for (int x = start_x + 1; x <  width - 1; x++) {
            mvwprintw(win, y, x," ");
        }
    }
    wrefresh(win);
}


struct tasks_informations{
char situation;
char name[300];
char deadline[11];
char subtasks[15][100];
char categories[15][100];
char description[2000];
int n_categories;
int n_subtasks;
int prioritys[4];
}tasks[50];

int highlight_pointer = 3;
int n_tasks = 0;

// first one ordered based on being made (automatically in the order of being made, but can be done by pressing o key)
// second one ordered by the given prioritys (can be done by pressing p key)
// for giving a priority to a task you can press y
// third one ordered based on the deadline press i
// fourth one ordered alphabetically (can be done by pressing u key)
int the_chosen_po = 1;



// a function to give the number of tasks from the json file to the file.
void number_of_tasks_saved()
{
    char *json_data = load_json_from_file("storage.json");
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("Error parsing JSON file.\n");
        return ;
    }

    cJSON *tasksArray = cJSON_GetObjectItem(root, "tasksArray");
    if (!tasksArray || !cJSON_IsArray(tasksArray)) {
        printf("Error: 'tasksArray' is not an array or does not exist.\n");
        cJSON_Delete(root);
        return ;
    }

    cJSON *number_of_tasks = cJSON_GetObjectItem(root, "number_of_tasks");
    n_tasks = number_of_tasks->valueint;

    cJSON_Delete(root);
}




int main()
{

//passing data by saved json file--------------------------------------------------------------------------------------------------------------------------------------------------------------
number_of_tasks_saved();

    // Load JSON data from file
    char *json_data = load_json_from_file("storage.json");
    if (json_data == NULL) {
        printf("Error loading JSON data.\n");
    }

    // Parse the JSON data
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("Error parsing JSON file.\n");
        free(json_data);
    }

    // Get the tasks array from the JSON
    cJSON *tasksArray = cJSON_GetObjectItem(root, "tasksArray");
    if (!tasksArray || !cJSON_IsArray(tasksArray)) {
        printf("Error: 'tasksArray' is not an array or does not exist.\n");
        cJSON_Delete(root);
        free(json_data);
    }

    for (int i = 0; i < n_tasks; i++) {
        if (i < cJSON_GetArraySize(tasksArray)) {
            cJSON *task = cJSON_GetArrayItem(tasksArray, i);

            cJSON *name = cJSON_GetObjectItem(task, "name");
            strcpy(tasks[i].name, name->valuestring);

            cJSON *description = cJSON_GetObjectItem(task, "description");
            strcpy(tasks[i].description, description->valuestring);

            cJSON *deadline = cJSON_GetObjectItem(task, "deadline");
            strcpy(tasks[i].deadline, deadline->valuestring);

            cJSON *situation = cJSON_GetObjectItem(task, "situation");
            if (situation->valueint == 1)
            {
                tasks[i].situation = 1;
            }
            else
            {
                tasks[i].situation = 0;
            }
            cJSON *n_subtasks = cJSON_GetObjectItem(task, "n_subtasks");
            tasks[i].n_subtasks = n_subtasks->valueint;

            cJSON *n_categories = cJSON_GetObjectItem(task, "n_categories");
            if (n_categories) tasks[i].n_categories = n_categories->valueint;

            // Handle subtasks
            cJSON *subtasks = cJSON_GetObjectItem(task, "subtasks");
                for (int j = 0; j < tasks[i].n_subtasks; j++) {
                    cJSON *subtask = cJSON_GetArrayItem(subtasks, j);
                    strcpy(tasks[i].subtasks[j], subtask->valuestring);
                }

            // Handle categories
            cJSON *categories = cJSON_GetObjectItem(task, "categories");
                for (int k = 1; k <= tasks[i].n_categories ; k++) {
                    cJSON *category = cJSON_GetArrayItem(categories, k);
                    strcpy(tasks[i].categories[k], category->valuestring);
            }

            // Handle priorities
            cJSON *priorities = cJSON_GetObjectItem(task, "priority");
                for (int a = 0; a < 4; a++)
                {
                    cJSON *priority = cJSON_GetArrayItem(priorities,a);
                    tasks[i].prioritys[a] = priority->valueint;
                }
        }
    }
    free(json_data);
    cJSON_Delete(root);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// base x and y are 214 and 64. //
int x_max = getting_window_x(214);
int y_max = getting_window_y(67);



//ncurses start *****************************************************************************************************************************************************************************
initscr();
start_color();
noecho();

int y_categories = 3*y_max/5-2*y_max/5;

WINDOW *win_subtasks = newwin(y_max-2*y_max/3,3*x_max/5,2*y_max/3,0);
WINDOW *win_tasks = newwin(2*y_max/3,3*x_max/5,0,0);
WINDOW *win_description = newwin(2*y_max/3,x_max-3*x_max/5,0,3*x_max/5);
WINDOW *win_categories = newwin(y_categories,x_max-3*x_max/5,2*y_max/3,3*x_max/5);
WINDOW *win_deadline = newwin(y_max-y_categories-2*y_max/3,x_max-3*x_max/5,2*y_max/3+y_categories,3*x_max/5);
refresh();


box(win_description,0,0);
wrefresh(win_description);
box(win_tasks,0,0);
wrefresh(win_tasks);
box(win_subtasks,0,0);
wrefresh(win_subtasks);
box(win_categories,0,0);
wrefresh(win_categories);
box(win_categories,0,0);
wrefresh(win_categories);
box(win_deadline,0,0);
wrefresh(win_deadline);


wprintw(win_tasks,"Tasks");
wrefresh(win_tasks);
wprintw(win_description,"Description");
wrefresh(win_description);
wprintw(win_categories,"Categories");
wrefresh(win_categories);
wprintw(win_subtasks,"Subtasks");
wrefresh(win_subtasks);
wprintw(win_deadline,"Deadline");
wrefresh(win_deadline);


int option = 0;

while (option != 113)
{
// typing the dead line //
clear_box_content(win_deadline,0,0,2*y_max/3,3*x_max/5);
if (tasks[highlight_pointer].deadline[0] == '\0')
    {
        clear_box_content(win_deadline,0,0,y_max-y_categories-2*y_max/3,x_max-3*x_max/5);
        mvwprintw(win_deadline,1,1,"NO deadline has been set yet!");
    }
else if (is_valid(tasks[highlight_pointer].deadline) == 0)
    {
        clear_box_content(win_deadline,0,0,y_max-y_categories-2*y_max/3,x_max-3*x_max/5);
        mvwprintw(win_deadline,1,1,"That's not valid read it like: yyyy/mm/dd\n and pay attention to the month and day you enter!");
    }
else
    {
        clear_box_content(win_deadline,0,0,y_max-y_categories-2*y_max/3,x_max-3*x_max/5);
        tasks[highlight_pointer].deadline[10] = '\0';
        for (int i = 0; i < 10; i++)
        {
        mvwprintw(win_deadline,1,1+i,"%c",tasks[highlight_pointer].deadline[i]);
        }
    }
box(win_deadline,0,0);
mvwprintw(win_deadline,0,0,"Deadline");
wrefresh(win_deadline);

// typing the situations //
int number_temp = n_tasks;
while (number_temp>0)
{
    mvwprintw(win_tasks,n_tasks-number_temp+1,3,"[ ]");
    if (tasks[n_tasks-number_temp].situation == 1)
    {
        mvwprintw(win_tasks,n_tasks-number_temp+1,4,"X");
    }
    wrefresh(win_tasks);
    number_temp--;
}

// typing the description //
if (tasks[highlight_pointer].description[0] != '\0')
{
clear_box_content(win_description,0,0,y_categories,x_max-3*x_max/5);
mvwprintw(win_description,1,1,"%s",tasks[highlight_pointer].description);
box(win_description,0,0);
mvwprintw(win_description,0,0,"Description");
wrefresh(win_description);
}
else
{
clear_box_content(win_description,0,0,y_categories,x_max-3*x_max/5);
mvwprintw(win_description,1,1,"NO description has been set yet.");
wrefresh(win_description);
}

// typing the categories //
clear_box_content(win_categories,0,0,y_categories,x_max-3*x_max/5);
for (int i = 1; i < 50; i++)
{
    if (tasks[highlight_pointer].categories[i][0] != '\0')
    {
        mvwprintw(win_categories,i,1,"%s",tasks[highlight_pointer].categories[i]);
    }
    else
    {
        break;
    }
}
box(win_categories,0,0);
mvwprintw(win_categories,0,0,"Categories");
wrefresh(win_categories);

// typing the subtasks //
clear_box_content(win_subtasks,0,0,y_max-2*y_max/3,3*x_max/5);
wrefresh(win_subtasks);
for (int j = 0; j < tasks[highlight_pointer].n_subtasks; j++)
{
    mvwprintw(win_subtasks,j+1,1,"%s",tasks[highlight_pointer].subtasks[j]);
    wrefresh(win_subtasks);
}

// typing tasks based on the priority //

// based on the order of being made
if (the_chosen_po == 1)
{
int y = 0;
    for (int l = 0; l < 50; l++)
    {
        for (int j = 0; j < n_tasks; j++)
        {
            if (tasks[j].prioritys[0] == l)
            {
                mvwprintw(win_tasks,y+1,1,"%d.",l);
                if (y == highlight_pointer)
                {
                    wattron(win_tasks,A_REVERSE);
                    mvwprintw(win_tasks,y+1,6,"%s",tasks[j].name);
                    wattroff(win_tasks,A_REVERSE);
                    wrefresh(win_tasks);
                    box(win_tasks,0,0);
                    wrefresh(win_tasks);
                    mvwprintw(win_tasks,0,0,"Tasks");
                    wrefresh(win_tasks);
                }
                else
                {
                    mvwprintw(win_tasks,y+1,6,"%s",tasks[j].name);
                    wrefresh(win_tasks);
                    box(win_tasks,0,0);
                    wrefresh(win_tasks);
                    mvwprintw(win_tasks,0,0,"Tasks");
                    wrefresh(win_tasks);
                }
                // the usage of tasks[49] is a temp task.
                tasks[49] = tasks[y];
                tasks[y] = tasks[j];
                tasks[j] = tasks[49];
                y++;
            }
        }
    }
}

// based on the setted priority
else if (the_chosen_po == 2)
{
int y = 0;
    for (int l = 0; l < 10; l++)
    {
        for (int j = 0; j < n_tasks; j++)
        {
            if (tasks[j].prioritys[1] == l)
            {
                mvwprintw(win_tasks,y+1,1,"%d.",l);
                if (y == highlight_pointer)
                {
                    wattron(win_tasks,A_REVERSE);
                    mvwprintw(win_tasks,y+1,6,"%s",tasks[j].name);
                    wattroff(win_tasks,A_REVERSE);
                    wrefresh(win_tasks);
                    box(win_tasks,0,0);
                    wrefresh(win_tasks);
                    mvwprintw(win_tasks,0,0,"Tasks");
                    wrefresh(win_tasks);
                }
                else
                {
                    mvwprintw(win_tasks,y+1,6,"%s",tasks[j].name);
                    wrefresh(win_tasks);
                    box(win_tasks,0,0);
                    wrefresh(win_tasks);
                    mvwprintw(win_tasks,0,0,"Tasks");
                    wrefresh(win_tasks);
                }
                // the usage of tasks[49] is a temp task.
                tasks[49] = tasks[y];
                tasks[y] = tasks[j];
                tasks[j] = tasks[49];
                y++;
            }
        }
    }
}

// based on the sooner deadline
else if (the_chosen_po == 3)
{
    for (int q = 0; q < n_tasks; q++)
    {
        tasks[q].prioritys[2] = q;
    }
    for (int i = 0; i < n_tasks; i++)
    {
        for (int j = i; j < n_tasks-1; j++)
        {
            if (sooner_deadline(tasks[j+1].deadline,tasks[i].deadline) == 1)
            {
                tasks[49].prioritys[2] = tasks[i].prioritys[2];
                tasks[i].prioritys[2] = tasks[j+1].prioritys[2];
                tasks[j+1].prioritys[2] = tasks[49].prioritys[2];
            }
            else
            {
            //  :)
            }
        }
    }
int y_p = 0;
    for (int p = 0; p < n_tasks; p++)
    {
        for (int j = p; j < n_tasks; j++)
        {
            if (tasks[j].prioritys[2] == p)
            {
            mvwprintw(win_tasks,p+1,1,"%d.",p+1);
            wrefresh(win_tasks);    
            if (p == highlight_pointer)
            {
                wattron(win_tasks,A_REVERSE);
                mvwprintw(win_tasks,p+1,6,"%s",tasks[p].name);
                wattroff(win_tasks,A_REVERSE);
                wrefresh(win_tasks);
                box(win_tasks,0,0);
                wrefresh(win_tasks);
                mvwprintw(win_tasks,0,0,"Tasks");
                wrefresh(win_tasks);
            }
            else
            {
                mvwprintw(win_tasks,p+1,6,"%s",tasks[p].name);
                wrefresh(win_tasks);
                box(win_tasks,0,0);
                wrefresh(win_tasks);
                mvwprintw(win_tasks,0,0,"Tasks");
                wrefresh(win_tasks);
            }
                // the usage of tasks[49] is a temp task.
                tasks[49] = tasks[y_p];
                tasks[y_p] = tasks[j];
                tasks[j] = tasks[49];
                y_p++;
            }
        }
    }
}
wrefresh(win_tasks);

// based on alphabet
if (the_chosen_po == 4)
{
    for (int m = 0;m < n_tasks;m++)
    {
        tasks[m].prioritys[3] = m;
    }
    for (int i = 0; i < n_tasks; i++)
    {
        for (int j = i; j < n_tasks; j++)
        {
            if (sooner_alphabetically(tasks[j+1].name,tasks[i].name) == 1)
            {
                tasks[49].prioritys[3] = tasks[i].prioritys[3];
                tasks[i].prioritys[3] = tasks[j+1].prioritys[3];
                tasks[j+1].prioritys[3] = tasks[49].prioritys[3];
            }
            else
            {
            //  :)
            }
        }
    }

int y_a = 0;
    for (int g = 0; g < n_tasks; g++)
    {
        for (int j = 0; j < n_tasks; j++)
        {
            if (tasks[j].prioritys[3] == g)
            {
                // the usage of tasks[49] is a temp task.
                tasks[49] = tasks[y_a];
                tasks[y_a] = tasks[j];
                tasks[j] = tasks[49];
                y_a++;
            }
        }
    }
refresh();

    for (int k = 0; k < n_tasks; k++)
    {
        mvwprintw(win_tasks,k+1,1,"%d.",k+1);
        wrefresh(win_tasks);
        if (k == highlight_pointer)
        {
            wattron(win_tasks,A_REVERSE);
            mvwprintw(win_tasks,k+1,6,"%s",tasks[k].name);
            wattroff(win_tasks,A_REVERSE);
            wrefresh(win_tasks);
            box(win_tasks,0,0);
            wrefresh(win_tasks);
            mvwprintw(win_tasks,0,0,"Tasks");
            wrefresh(win_tasks);
        }
        else
        {
            mvwprintw(win_tasks,k+1,6,"%s",tasks[k].name);
            wrefresh(win_tasks);
            box(win_tasks,0,0);
            wrefresh(win_tasks);
            mvwprintw(win_tasks,0,0,"Tasks");
            wrefresh(win_tasks);
        }
    }
}


option = getch();
options(option,win_tasks,y_max,x_max,win_deadline,win_description,win_categories,win_subtasks);
}

endwin();//ncurses end **********************************************************************************************************************************************************************
return 0;
}




void options(int option,WINDOW *win_tasks,int y_max,int x_max,WINDOW *win_deadline,WINDOW *win_description,WINDOW *win_categories,WINDOW *win_subtasks)
{
switch (option)
    {
    // a
    case 97:
        n_tasks ++;
        highlight_pointer = n_tasks-1;
        tasks[n_tasks-1].prioritys[0] = n_tasks;
        wrefresh(win_tasks);
        int user_input_character = 0;
        int x_counter=1;
        while (user_input_character != 10)
        {
            user_input_character = getch();
            if (user_input_character == 127 && x_counter>1)
            {
                x_counter--;
                tasks[n_tasks-1].name[x_counter] = ' '; 
                wattron(win_tasks,A_REVERSE);
                mvwprintw(win_tasks,n_tasks,x_counter," ");
                wattroff(win_tasks,A_REVERSE);
                wrefresh(win_tasks);
            }
            else if (user_input_character == 127)
            {
            // :)
            }
            else
            {
                tasks[n_tasks-1].name[x_counter-1] = user_input_character;
                x_counter++;
                wattron(win_tasks,A_REVERSE);
                mvwprintw(win_tasks,n_tasks,x_counter-1,"%c",user_input_character);
                wattroff(win_tasks,A_REVERSE);
                wrefresh(win_tasks);
            }
        }
        refresh();
        break;

    // d
    case 100:    
        if (n_tasks > 0)
        {     
        for (int i = highlight_pointer; i < n_tasks; i++)
        {
            tasks[i] = tasks[i+1];
        }
        n_tasks--;
        clear_box_content(win_tasks,n_tasks,0,2*y_max/3-n_tasks,3*x_max/5-1);
        if (highlight_pointer == n_tasks)
        {
            highlight_pointer--;
        }
        }
        refresh();
        break;
    // e
    case 101:
        if (tasks[highlight_pointer].name[0] != '\0')
        {
        refresh();
        int user_choice = getch();
        int _counter = 4;
        while (user_choice != 10)
        {
            switch (user_choice)
            {
            case KEY_UP:
                break;
            case KEY_DOWN:
                break;
            case KEY_LEFT:
                _counter--;
                break;
            case KEY_RIGHT:
                _counter++;
                break;
            case 127:
                tasks[highlight_pointer].name[_counter-2] = ' ';
                wattron(win_tasks,A_REVERSE);
                mvwprintw(win_tasks,highlight_pointer+1,_counter-1+3," ");
                wattroff(win_tasks,A_REVERSE);
                wrefresh(win_tasks);
                _counter--;
                break;
            default:
                tasks[highlight_pointer].name[_counter-1] = user_choice;
                wattron(win_tasks,A_REVERSE);
                mvwprintw(win_tasks,highlight_pointer+1,_counter+3,"%c",user_choice);
                wattroff(win_tasks,A_REVERSE);
                wrefresh(win_tasks);
                _counter++;
                break;
            }
        user_choice = getch();
        }
        }
        break;

    // j
    case 106:
        if (highlight_pointer<n_tasks-1)
        {
        highlight_pointer++;
        wrefresh(win_deadline);
        }
        break;

    // k
    case 107:
        if (highlight_pointer>0)
        {    
        highlight_pointer--;
        wrefresh(win_deadline);
        }
        break;




// the whole subtask part //
    // l
    case 108:
        if (tasks[0].name[0] != '\0')
        {
        refresh();
        int highlight_subtask = 1;
        int choice_subtask = getch();
        while (choice_subtask != 104)
        {

            if (choice_subtask == 97)
            {
                tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks][0] = '[';
                tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks][1] = ' ';
                tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks][2] = ']';
                int subtask_counter_x = 1;
                tasks[highlight_pointer].n_subtasks++;
                refresh();
                highlight_subtask = tasks[highlight_pointer].n_subtasks;
                int user_input_subtask = getch();
                while (user_input_subtask != 10)
                { 
                if (user_input_subtask == 127 && subtask_counter_x > 1)
                {
                tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks-1][subtask_counter_x+1] = ' ';
                wattron(win_subtasks,A_REVERSE);
                mvwprintw(win_subtasks,tasks[highlight_pointer].n_subtasks,subtask_counter_x-1," ");
                wattroff(win_subtasks,A_REVERSE);
                wrefresh(win_subtasks);
                subtask_counter_x--;
                }
                else if (user_input_subtask == 127)
                {
                //    :)
                }
                else
                {
                tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks-1][subtask_counter_x+2] = user_input_subtask;          
                wattron(win_subtasks,A_REVERSE);
                mvwprintw(win_subtasks,tasks[highlight_pointer].n_subtasks,subtask_counter_x,"%C",user_input_subtask);
                wattroff(win_subtasks,A_REVERSE);
                wrefresh(win_subtasks);
                subtask_counter_x++;
                }
                user_input_subtask = getch();
                }
            }
            else if (choice_subtask == 32)
            {
                if (tasks[highlight_pointer].subtasks[highlight_subtask-1][1] != 'X')
                {
                tasks[highlight_pointer].subtasks[highlight_subtask-1][1] = 'X';
                }
                else
                {
                tasks[highlight_pointer].subtasks[highlight_subtask-1][1] = ' ';
                }
            }
            else if (choice_subtask == 106)
            {
                if (highlight_subtask<tasks[highlight_pointer].n_subtasks)
                {
                highlight_subtask++;
                wrefresh(win_subtasks);
                }
            }
            else if (choice_subtask == 107)
            {
                if (highlight_subtask>1)
                {
                highlight_subtask--;
                wrefresh(win_subtasks);
                }
            }
            else if (choice_subtask == 100 && tasks[highlight_pointer].n_subtasks > 0)
            {
                for (int i = highlight_subtask-1; i < tasks[highlight_pointer].n_subtasks; i++)
                {
                    for (int j = 0; j <= 100; j++)
                    {
                        tasks[highlight_pointer].subtasks[i][j] = tasks[highlight_pointer].subtasks[i+1][j];
                    }
                }
                memset(tasks[highlight_pointer].subtasks[tasks[highlight_pointer].n_subtasks],'\0',100);
                tasks[highlight_pointer].n_subtasks --;
                if (highlight_subtask == tasks[highlight_pointer].n_subtasks+1)
                {
                    highlight_subtask--;
                    wrefresh(win_subtasks);
                }
            }
            else
            {
            // :)
            }
            
        clear_box_content(win_subtasks,0,0,y_max-2*y_max/3,3*x_max/5);
        for (int m = 1; m <= tasks[highlight_pointer].n_subtasks; m++)
        {
            if (m == highlight_subtask)
            {
                wattron(win_subtasks,A_REVERSE);
                mvwprintw(win_subtasks,m,1,"%s",tasks[highlight_pointer].subtasks[m-1]);
                wattroff(win_subtasks,A_REVERSE);
                wrefresh(win_subtasks);
            }
            else
            {
                mvwprintw(win_subtasks,m,1,"%s",tasks[highlight_pointer].subtasks[m-1]);
                wrefresh(win_subtasks);
            }
        }
        box(win_subtasks,0,0);
        wrefresh(win_subtasks);
        mvwprintw(win_subtasks,0,0,"Subtasks");
        wrefresh(win_subtasks);
        choice_subtask = getch();
        }
        }
        break;
// end of subtask part //




    // n
    case 110:
        if (tasks[highlight_pointer].name[0] != '\0')
        {
        clear_box_content(win_deadline,0,0,y_max,x_max);
        wrefresh(win_deadline);
        box(win_deadline,0,0),
        wrefresh(win_deadline);
        mvwprintw(win_deadline,0,0,"Deadline");
        wrefresh(win_deadline);
        int user_input = getch();
        int counter = 0;
        while (user_input != 10)
        {
            if (user_input == 127 && counter > 0)
            {
                wattron(win_deadline,A_REVERSE);
                mvwprintw(win_deadline,1,counter," ");
                wattroff(win_deadline,A_REVERSE);
                counter--;
                tasks[highlight_pointer].deadline[counter] = ' ';
                wrefresh(win_deadline);
            }
            else if (user_input == 127)
            {
            // :)
            }   
            else
            {
                tasks[highlight_pointer].deadline[counter] = user_input;
                wattron(win_deadline,A_REVERSE);
                mvwprintw(win_deadline,1,counter+1,"%c",user_input);
                wattroff(win_deadline,A_REVERSE);
                wrefresh(win_deadline);
                counter++;
            }
            user_input = getch();
        }
        }
        break;

    // r
    case 114:
        if (tasks[highlight_pointer].name[0] != '\0')
        {
        clear_box_content(win_description,0,0,3*y_max/5-2*y_max/5,x_max-3*x_max/5);
        memset(tasks[highlight_pointer].description,'\0',2000);
        int dis_input = getch();
        int counter_dis = 0;
        int number_line = 1;
        int counter_d = 0;
        while (dis_input != 27)
        {
            if (dis_input == 10)
            {
                counter_dis = 1;
                number_line ++;
                tasks[highlight_pointer].description[counter_d] = '\n';
                tasks[highlight_pointer].description[counter_d+1] = ' ';
                counter_dis+= -1;
                counter_d+=2;
                box(win_description,0,0);
                mvwprintw(win_description,0,0,"Description");
                wrefresh(win_description);
            }
            else if (dis_input == 127 && counter_dis > 0)
            {
                counter_dis--;
                counter_d--;
                tasks[highlight_pointer].description[counter_d] = ' ';
                mvwprintw(win_description,number_line,counter_dis+1," ");
                wrefresh(win_description);
            }
            else if (dis_input == 127)
            {
            // :)
            }
            else
            {
                wattron(win_description,A_REVERSE);
                mvwprintw(win_description,number_line,counter_dis+1,"%c",dis_input);
                wattroff(win_description,A_REVERSE);
                tasks[highlight_pointer].description[counter_d] = dis_input;
                counter_dis++;
                counter_d++;
                wrefresh(win_description);
            }
            refresh();
            dis_input = getch();
        }
        }
        break;




// the whole category part //
    // c
    case 99:
    if(tasks[highlight_pointer].name[0] != '\0')
    {
        refresh();
        int category_ptr = 1;
        int choice_category = getch();
    while (choice_category != 99)
    {
        if (choice_category == 97 && tasks[highlight_pointer].n_categories < 3*y_max/5-2*y_max/5-2)
        {
            tasks[highlight_pointer].n_categories ++;
            category_ptr = tasks[highlight_pointer].n_categories;
            int categories_x = 0;
            while (choice_category != 10)
            {
                choice_category = getch();
                if (choice_category == 127 && categories_x>0)
                {
                    tasks[highlight_pointer].categories[category_ptr][categories_x] = ' ';
                    wattron(win_categories,A_REVERSE);
                    mvwprintw(win_categories,tasks[highlight_pointer].n_categories,categories_x," ");
                    wattroff(win_categories,A_REVERSE);
                    wrefresh(win_categories);
                    categories_x--;
                }
                else if (choice_category == 127)
                {
                // :)
                }
                else
                {
                    tasks[highlight_pointer].categories[category_ptr][categories_x] = choice_category;
                    wattron(win_categories,A_REVERSE);
                    mvwprintw(win_categories,tasks[highlight_pointer].n_categories,categories_x+1,"%c",choice_category);
                    wattroff(win_categories,A_REVERSE);
                    wrefresh(win_categories);
                    categories_x++;
                }
            }
        }

        else if (choice_category == 106)
        {
            if (category_ptr<tasks[highlight_pointer].n_categories)
            {
            category_ptr++;
            wrefresh(win_categories);
            }
        }

        else if (choice_category == 107)
        {
            if (category_ptr>1)
            {    
            category_ptr--;
            wrefresh(win_categories);
            }
        }
        else if (choice_category == 100 && tasks[highlight_pointer].n_categories > 0) 
        {
            for (int i = category_ptr; i < tasks[highlight_pointer].n_categories; i++)
            {
                for (int j = 0; j <= 100; j++)
                {
                    tasks[highlight_pointer].categories[i][j] = tasks[highlight_pointer].categories[i+1][j];
                }
            }
            memset(tasks[highlight_pointer].categories[tasks[highlight_pointer].n_categories],'\0',100);
            tasks[highlight_pointer].n_categories --;
            if (category_ptr == tasks[highlight_pointer].n_categories+1)
            {
                category_ptr--;
                wrefresh(win_categories);
            }  
        }
        else
        {
        // :)
        }

        clear_box_content(win_categories,0,0,3*y_max/5-2*y_max/5,x_max-3*x_max/5-1);
        for (int j = 0; j <= tasks[highlight_pointer].n_categories; j++)
        {
            if (j == category_ptr)
            {
                wattron(win_categories,A_REVERSE);
                mvwprintw(win_categories,j,1,"%s",tasks[highlight_pointer].categories[j]);
                wattroff(win_categories,A_REVERSE);
                wrefresh(win_categories);
            }
            else
            {
                mvwprintw(win_categories,j,1,"%s",tasks[highlight_pointer].categories[j]);
                wrefresh(win_categories);
            }
        }
        box(win_categories,0,0);
        wrefresh(win_categories);
        mvwprintw(win_categories,0,0,"Categories");
        wrefresh(win_categories);
        choice_category = getch();
        }
        }
        break;
// end of category part //




    // space
    case 32:
        if (tasks[highlight_pointer].situation == 1)
        {
            tasks[highlight_pointer].situation = 0;
        }
        else
        {
        tasks[highlight_pointer].situation = 1;
        }
        break;

// storing part
    // w
    case 119:
    refresh();
    char *json_data = load_json_from_file("storage.json");
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("Error parsing JSON file.\n");
        return;
    }

    cJSON *tasksArray = cJSON_GetObjectItem(root, "tasksArray");
    if (!tasksArray || !cJSON_IsArray(tasksArray)) {
        printf("Error: 'tasksArray' is not an array or does not exist.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *number_of_tasks = cJSON_GetObjectItem(root, "number_of_tasks");
    cJSON_SetNumberValue(number_of_tasks,n_tasks);

    for (int i = 0; i < n_tasks; i++) {

            cJSON *task = cJSON_GetArrayItem(tasksArray, i);

            cJSON_ReplaceItemInObject(task, "name", cJSON_CreateString(tasks[i].name));
            cJSON_ReplaceItemInObject(task, "deadline", cJSON_CreateString(tasks[i].deadline));
            cJSON_ReplaceItemInObject(task, "description", cJSON_CreateString(tasks[i].description));

            
            if (tasks[i].situation == 1)
            {
            cJSON_ReplaceItemInObject(task, "situation", cJSON_CreateNumber(1));
            }
            else
            {
            cJSON_ReplaceItemInObject(task, "situation",cJSON_CreateNumber(0));
            }
            cJSON_ReplaceItemInObject(task, "n_subtasks", cJSON_CreateNumber(tasks[i].n_subtasks));
            cJSON_ReplaceItemInObject(task, "n_categories", cJSON_CreateNumber(tasks[i].n_categories));


        cJSON *subtasksArray = cJSON_GetObjectItem(task,"subtasks");
        for (int j = 0; j < tasks[i].n_subtasks; j++) {
            cJSON_ReplaceItemInArray(subtasksArray, j, cJSON_CreateString(tasks[i].subtasks[j]));
        }

// the first item remains empty i dont know why!
        cJSON *categoriesArray = cJSON_GetObjectItem(task,"categories");
        for (int k = 0; k <= tasks[i].n_categories; k++) {
            cJSON_ReplaceItemInArray(categoriesArray, k, cJSON_CreateString(tasks[i].categories[k]));
        }   

        cJSON *prioritysArray = cJSON_GetObjectItem(task,"priority");
        for (int q = 0; q < 4; q++)
        {
            cJSON_ReplaceItemInArray(prioritysArray,q,cJSON_CreateNumber(tasks[i].prioritys[q]));
        }
    
}
    char *updated_json_data = cJSON_Print(root);
    save_json_to_file("storage.json", updated_json_data);

    cJSON_Delete(root);
        break;

    // y
    case 121:
        refresh();
        int temp_number;
        getting_priority(&temp_number,win_tasks,highlight_pointer);
        tasks[highlight_pointer].prioritys[1] = temp_number;
        break;

    // o
    case 111:
        the_chosen_po = 1;
        break;

    // p
    case 112:
        the_chosen_po = 2;
        break;

    // i
    case 105:
        the_chosen_po = 3;
        break;

    // u
    case 117:
        the_chosen_po = 4;
        break;

    // q
    case 113:
        break;

    default:
        option = getch();
        options(option,win_tasks,y_max,x_max,win_deadline,win_description,win_categories,win_subtasks);
        break;
    }
}