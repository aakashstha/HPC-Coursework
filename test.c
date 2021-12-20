// 2 Blur
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lodepng.h"

int i;
unsigned char *ImageIn, *ImageOut;
unsigned int width, height, error;
unsigned int r = 0, g = 0, b = 0, t = 0;
unsigned int r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0;

void RGB_SumDivide(int j, int d)
{
    if (d == 4)
    {
        r = ImageIn[r1] + ImageIn[r2] + ImageIn[r3] + ImageIn[r4];
        g = ImageIn[r1 + 1] + ImageIn[r2 + 1] + ImageIn[r3 + 1] + ImageIn[r4 + 1];
        b = ImageIn[r1 + 2] + ImageIn[r2 + 2] + ImageIn[r3 + 2] + ImageIn[r4 + 2];
    }
    else if (d == 6)
    {
        r = ImageIn[r1] + ImageIn[r2] + ImageIn[r3] + ImageIn[r4] + ImageIn[r5] + ImageIn[r6];
        g = ImageIn[r1 + 1] + ImageIn[r2 + 1] + ImageIn[r3 + 1] + ImageIn[r4 + 1] + ImageIn[r5 + 1] + ImageIn[r6 + 1];
        b = ImageIn[r1 + 2] + ImageIn[r2 + 2] + ImageIn[r3 + 2] + ImageIn[r4 + 2] + ImageIn[r5 + 2] + ImageIn[r6 + 2];
    }
    else if (d == 9)
    {
        r = ImageIn[r1] + ImageIn[r2] + ImageIn[r3] + ImageIn[r4] + ImageIn[r5] + ImageIn[r6] + ImageIn[r7] + ImageIn[r8] + ImageIn[r9];
        g = ImageIn[r1 + 1] + ImageIn[r2 + 1] + ImageIn[r3 + 1] + ImageIn[r4 + 1] + ImageIn[r5 + 1] + ImageIn[r6 + 1] + ImageIn[r7 + 1] + ImageIn[r8 + 1] + ImageIn[r9 + 1];
        b = ImageIn[r1 + 2] + ImageIn[r2 + 2] + ImageIn[r3 + 2] + ImageIn[r4 + 2] + ImageIn[r5 + 2] + ImageIn[r6 + 2] + ImageIn[r7 + 2] + ImageIn[r8 + 2] + ImageIn[r9 + 2];
    }

    t = ImageIn[j + 3];
    ImageOut[j] = r / d;
    ImageOut[j + 1] = g / d;
    ImageOut[j + 2] = b / d;
    ImageOut[j + 3] = t;
}

void processImage()
{
    error = lodepng_decode32_file(&ImageIn, &width, &height, "4x4.png");
    if (error)
    {
        printf("%d %s", error, lodepng_error_text(error));
    }
    ImageOut = malloc(width * height * 4 * sizeof(char));

    int row = 4 * width;
    int full = row * height;
    for (i = 0; i < row * height; i += 4)
    {
        printf("Hello!! ===== %d\n", i);
        // 4 corner pixel
        if (i == 0)
        {
            r1 = i, r2 = i + 4, r3 = row, r4 = row + 4;
            RGB_SumDivide(i, 4);
        }
        else if (i == (row - 4)) //i==16
        {
            r1 = i - 4, r2 = i, r3 = row * 2 - 4 - 4, r4 = row * 2 - 4;
            RGB_SumDivide(i, 4);
        }
        else if (i == (row * height) - row) //i==80
        {
            r1 = full - (row * 2), r2 = full - (row * 2) + 4, r3 = i, r4 = i + 4;
            RGB_SumDivide(i, 4);
        }
        else if (i == (row * height - 4)) //i==96
        {
            r1 = full - (row * 2) - (4 * 2), r2 = full - (row * 2) - 4, r3 = i - 4, r4 = i;
            RGB_SumDivide(i, 4);
        }
        else if (i < (row)) // Fisrt Edge Row
        {
            r1 = i - 4, r2 = i, r3 = i + 4, r4 = row + (i - 4), r5 = row + i, r6 = row + i + 4;
            RGB_SumDivide(i, 6);
        }
        else if (i == (row * (int)(i / row))) // Left Edge Row
        {
            r1 = i - row, r2 = i - row + 4, r3 = i, r4 = i + 4, r5 = i + row, r6 = i + row + 4;
            RGB_SumDivide(i, 6);
        }

        else if (i == row * (int)((i + row) / row) - 4) // Rigth Edge Row
        {
            r1 = i - row - 4, r2 = i - row, r3 = i - 4, r4 = i, r5 = i + row - 4, r6 = i + row;
            RGB_SumDivide(i, 6);
        }
        else if (i > (row * height) - row) // Bottom Edge Row
        {
            r1 = i - row - 4, r2 = i - row, r3 = i - row + 4, r4 = i - 4, r5 = i, r6 = i + 4;
            RGB_SumDivide(i, 6);
        }
        else // All Center
        {
            r1 = i - row + 4, r2 = i + 4 - row + 4, r3 = i + 4 + 4 - row + 4, r4 = i - 4, r5 = i, r6 = i + 4, r7 = i + row - 4, r8 = i + row, r9 = i + row + 4;
            RGB_SumDivide(i, 9);
        }
        //printf("%d %d %d %d\n", ImageIn[i], ImageIn[i + 1], ImageIn[i + 2], ImageIn[i + 3]);
    }
    lodepng_encode32_file("wow.png", ImageOut, width, height);
    printf("\n\n%d %d\n", width, height);
}

void main()
{
    processImage();
}