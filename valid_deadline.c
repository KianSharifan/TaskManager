#include <stdio.h>
#include <ctype.h>

int is_valid(char *deadline);


int is_valid(char *deadline)
{
if (deadline[4] == '/' && deadline[7] == '/')
{
    if (isdigit(deadline[0]) && isdigit(deadline[1]) && isdigit(deadline[2]) && isdigit(deadline[3]) && isdigit(deadline[5]) && isdigit(deadline[6]) && isdigit(deadline[8]) && isdigit(deadline[9]))
    {
        if (deadline[5] == '1')
        {
            if (deadline[6] == '1' || deadline[6] == '2' || deadline[6] == '0')
            {
                if (deadline[8] == '0' || deadline[8] =='1' || deadline[8]=='2')
                {
                    return 1;
                }
                else if (deadline[8] == '3')
                {
                    if (deadline[9] == '0' || deadline[9] == '1')
                    {
                        return 1;
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
            else
            {
                return 0;
            }
        }
        else if (deadline[5] == '0')
        {
            if (deadline[8] == '0' || deadline[8] =='1' || deadline[8]=='2')
            {
                return 1;
            }
            else if (deadline[8] == '3')
            {
                if (deadline[9] == '0' || deadline[9] == '1')
                {
                    return 1;
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
else
{
    return 0;
}
}