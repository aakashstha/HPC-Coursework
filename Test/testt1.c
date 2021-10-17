#include <stdio.h>

void main()
{
    FILE *file = NULL;
    int rows1, cols1, rows2, cols2;
    int row, col;
    double matval = 0.0;

    file = fopen("SampleMatricesWithErrors.txt", "r");
    while (!feof(file))
    {

        // To store first matrix
        fscanf(file, "%d,%d", &rows1, &cols1);
        printf("Rows1: %d, Cols1: %d\n", rows1, cols1);
        double A[rows1][cols1];
        for (row = 0; row < rows1; row++)
        {
            for (col = 0; col < cols1; col++)
            {
                fscanf(file, "%lf,", &matval);
                A[row][col] = matval;
            }
        }

        // To store second matrix
        fscanf(file, "%d,%d", &rows2, &cols2);
        printf("Rows2: %d, Cols2: %d\n", rows2, cols2);
        double B[rows2][cols2];
        for (row = 0; row < rows2; row++)
        {
            for (col = 0; col < cols2; col++)
            {
                fscanf(file, "%lf,", &matval);
                B[row][col] = matval;
            }
        }

        // To stop repeating the last matrix twice
        if (feof(file))
        {
            break;
        }

        // To store matrix multiplication
        double C[rows1][cols2];
        for (row = 0; row < rows1; row++)
        {
            for (col = 0; col < cols2; col++)
            {
                // To initialize = 0.0 first in all of matrix multiplication array
                C[row][col] = 0.0;
                // here cols1 or rows2 both can do the task coz they are same.
                for (int k = 0; k < cols1; k++)
                {
                    C[row][col] += A[row][k] * B[k][col];
                }
                printf("%lf\t", C[row][col]);
            }
            printf("\n");
        }
        printf("\n");
    }
    fclose(file);
}
