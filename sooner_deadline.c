#include <ncurses.h>
#include <stdio.h>

int sooner_deadline(char *deadline1,char *deadline2);


int sooner_deadline(char *deadline1,char *deadline2)
{
    if (*deadline1 < *deadline2)
    {
        return 1;
    }
    else if (*deadline1 == *deadline2)
    {
        if (*(deadline1+1) < *(deadline2+1))
        {
            return 1;
        }
        else if (*(deadline1+1) == *(deadline2+1))
        {
            if (*(deadline1+2) < *(deadline2+2))
            {
                return 1;
            }
            else if (*(deadline1+2) == *(deadline2+2))
            {
                if (*(deadline1+3) < *(deadline2+3))
                {
                    return 1;
                }
                else if (*(deadline1+3) == *(deadline2+3))
                {
                    if (*(deadline1+5) < *(deadline2+5))
                    {
                        return 1;
                    }
                    else if (*(deadline1+5) == *(deadline2+5))
                    {
                        if (*(deadline1+6) < *(deadline2+6))
                        {
                            return 1;
                        }
                        else if (*(deadline1+6) == *(deadline2+6))
                        {
                            if (*(deadline1+8) < *(deadline2+8))
                            {
                                return 1;
                            }
                            else if (*(deadline1+8) == *(deadline2+8))
                            {
                                if (*(deadline1+9) < *(deadline2+9))
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