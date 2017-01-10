#include <stdio.h>
#include "conio.h"


int main()
{
    while (1)
    {
        int key;
        key = _getch();
        printf("0x%x ", key);
    }
}