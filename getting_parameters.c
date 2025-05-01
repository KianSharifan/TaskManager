#include <stdio.h>

int getting_window_x(int base_x);
int getting_window_y(int base_y);


int getting_window_x(int x)
{
    int x_answer;
    printf("Hello there\nWelcome please give me the x of your terminal:(default is 214 ,press 0 for that)");
    scanf("%d",&x_answer);
    if (x_answer!=0)
    {
        x = x_answer;
    }
    return x;
}
int getting_window_y(int y)
{
    int y_answer;
    printf("Now give me the y of your terminal please:(default 64 ,press 0 for that)");
    scanf("%d",&y_answer);
    if (y_answer!=0)
    {
        y = y_answer;
    }
    return y;
}