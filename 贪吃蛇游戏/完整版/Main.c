#include "Snake.h"
#include "Snake.c"

int main()
{
    while (1)
    {
        StartGame();
        RunGame();
        EndGame();
    }
    system("pause");
    return 0;
}