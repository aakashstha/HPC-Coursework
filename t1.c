#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *mult(void *arg)
{
    double *data = (double *)arg;
    double k = 0;
    int i = 0;

    int x = data[0];
    for (i = 1; i <= x; i++)
    {
        k += data[i] * data[i + x];
    }
    double *p = (double *)malloc(sizeof(double));
    *p = k;

    pthread_exit(p);
}

void main()
{
    FILE *file, *fileStore = NULL;
    int row, col; // to use in loops
    int rows1, cols1, rows2, cols2;
    double matval = 0.0;

    //file = fopen("SampleMatricesWithErrors.txt", "r");
    file = fopen("z_new.txt", "r");
    fileStore = fopen("z_matrixresults2049699.txt", "w");

    while (fscanf(file, "%d,%d", &rows1, &cols1) != EOF)
    {

        // To store first matrix A
        // double A[rows1][cols1];
        double *A[rows1];
        for (row = 0; row < rows1; row++)
        {
            // to dynamically allocate memory
            A[row] = malloc(cols1 * sizeof(double *));
            for (col = 0; col < cols1; col++)
            {
                fscanf(file, "%lf,", &matval);
                A[row][col] = matval;
            }
        }

        // To store second matrix B
        fscanf(file, "%d,%d", &rows2, &cols2);
        double *B[rows2];
        for (row = 0; row < rows2; row++)
        {
            B[row] = malloc(cols2 * sizeof(double *));
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

        int n = 2;

        pthread_t threads[n];
        double *data = NULL;
        int k;
        int count = 0;

        // To store and perform matrix multiplication C
        fprintf(fileStore, "Resultant Matrix: %d,%d\n", rows1, cols2);
        double C[rows1][cols2];
        for (row = 0; row < rows1; row++)
        {
            for (col = 0; col < cols2; col++)
            {
                // keeping 1st mat row and 2nd mat column
                // (cols1 * row2 +1) OR (cols1 * 2 +1) same
                data = malloc((cols1 * 2 + 1) * sizeof(double));
                data[0] = cols1;

                for (k = 0; k < cols1; k++)
                {
                    data[k + 1] = A[row][k];
                }
                for (k = 0; k < rows2; k++)
                {
                    data[k + cols1 + 1] = B[k][col];
                }

                if (count < n)
                {
                    pthread_create(&threads[count], NULL, mult, (void *)(data));
                    count++;
                }

                if (count == n || col == cols2 - 1)
                {
                    for (int i = 0; i < count; i++)
                    {
                        void *k;

                        pthread_join(threads[i], &k);
                        double *p = (double *)k;
                        fprintf(fileStore, "%lf\t", *p);
                    }
                    count = 0;
                }

                // // To initialize = 0.0 first in all of matrix multiplication array
                // C[row][col] = 0.0;
                // // here cols1 or rows2 both can do the task coz they are same.
                // for (int k = 0; k < cols1; k++)
                // {
                //     C[row][col] += A[row][k] * B[k][col];
                // }
                // fprintf(fileStore, "%lf\t", C[row][col]);
            }
            fprintf(fileStore, "\n");
        }
        fprintf(fileStore, "\n");
        break;
    }
    printf("Done.\n");
    fclose(file);
}
