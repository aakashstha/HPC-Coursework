#include <stdio.h>

void main()
{
    FILE *file, *fileStore = NULL;
    int row, col; // to use in loops
    int rows1, cols1, rows2, cols2; 
    double matval = 0.0;

    file = fopen("SampleMatricesWithErrors.txt", "r");
    //file = fopen("new.txt", "r");
    fileStore = fopen("z_matrixresults2049699.txt", "w");

    while (fscanf(file, "%d,%d", &rows1, &cols1) != EOF)
    {
        // To store first matrix A
        double A[rows1][cols1];
        for (row = 0; row < rows1; row++)
        {
            for (col = 0; col < cols1; col++)
            {
                fscanf(file, "%lf,", &matval);
                A[row][col] = matval;
            }
        }

        // To store second matrix B
        fscanf(file, "%d,%d", &rows2, &cols2);
        double B[rows2][cols2];
        for (row = 0; row < rows2; row++)
        {
            for (col = 0; col < cols2; col++)
            {
                fscanf(file, "%lf,", &matval);
                B[row][col] = matval;
            }
        }

        // if matrix cannot be multiplied like 3x3 by 2x2 then break the iteration
        if (cols1 != rows2)
        {
            printf("Matrix A is %dx%d\n", rows1, cols1);
            printf("Matrix B is %dx%d\n", rows2, cols2);
            printf("Error Message:: The matrix A and B cannot be multiplied.\n\n");
            continue;
        }

        // To store matrix multiplication C
        fprintf(fileStore, "Result Matrix: %d,%d\n", rows1, cols2);
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
                fprintf(fileStore, "%lf\t", C[row][col]);
            }
            fprintf(fileStore, "\n");
        }
        fprintf(fileStore, "\n");
    }
    printf("Done.\n");
    fclose(file);
}
