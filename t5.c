#include <stdio.h>
#include <stdlib.h>

int row, col;
void passDoublePointer(int **dPtr)
{
    for (row = 0; row < 2; row++)
    {
        for (col = 0; col < 2; col++)
        {
            printf("%d\n", dPtr[row][col]);
        }
    }
}

int main()
{
    int *A[2];
    for (row = 0; row < 2; row++)
    {
        A[row] = malloc(2 * sizeof(int *));
        for (col = 0; col < 2; col++)
        {
            A[row][col] = row * col;
        }
    }

    passDoublePointer(A);
}
