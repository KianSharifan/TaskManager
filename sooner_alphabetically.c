#include <ncurses.h>
#include <stdio.h>

int sooner_alphabetically(char *name1,char *name2);

int sooner_alphabetically(char *name1,char *name2)
{

for (int i = 0;i<100;i++)
{
    if (*name1 != '\0' && *name2 != '\0')
    {
    if (*name1 < *name2)
    {
        return 1;
    }
    else if (*name1 == *name2)
    {
        name1++;
        name2++;
    }
    else
    {
        return 0;
    }
    }
    else
    {
        return 0;
    }
}
return 0;
}