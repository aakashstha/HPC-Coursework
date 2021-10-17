#include <stdio.h>

void main()
{
    FILE *file, *fileStore = NULL;
    int rows1, cols1, rows2, cols2;
    int row, col;
    double matval = 0.0;

    file = fopen("SampleMatricesWithErrors.txt", "r");
    fileStore = fopen("z_StoreMatrixResult.txt", "w");
    //file = fopen("new.txt", "r");

    while (fscanf(file, "%d,%d", &rows1, &cols1) != EOF)
    {

        // To store first matrix
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
        double B[rows2][cols2];
        for (row = 0; row < rows2; row++)
        {
            for (col = 0; col < cols2; col++)
            {
                fscanf(file, "%lf,", &matval);
                B[row][col] = matval;
            }
        }

        // To store matrix multiplication
        double C[rows1][cols2];
        fprintf(fileStore, "Result Matrix: %d,%d\n", rows1, cols2);
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
