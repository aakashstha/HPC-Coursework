#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime_api.h>

#include "lodepng.h"

/*
    Example:-
     To Compile = nvcc task4.cu -o xyz lodepng.cpp
     To Run = ./xyz

	 The output of this task-4 program is a png image name = "WOW_Image.png".
*/
__global__ void boxBlur(unsigned char *gpu_imageOuput, unsigned char *gpu_imageInput, int width, int height);
__device__ void RGB_SumDivide(int j, int d, unsigned char *gpu_imageOuput, unsigned char *gpu_imageInput, int r1, int r2, int r3, int r4, int r5, int r6, int r7, int r8, int r9);

int main(int argc, char **argv)
{
	unsigned int error, encError, width, height;
	unsigned char *image;
	const char *filename = "Data/image.png";
	const char *newFileName = "WOW_Image.png";

	error = lodepng_decode32_file(&image, &width, &height, filename);
	if (error)
		printf("error %u: %s\n", error, lodepng_error_text(error));

	const int ARRAY_SIZE = width * height * 4;
	const int ARRAY_BYTES = ARRAY_SIZE * sizeof(unsigned char);

	unsigned char host_imageInput[ARRAY_SIZE * 4];
	unsigned char host_imageOutput[ARRAY_SIZE * 4];

	for (int i = 0; i < ARRAY_SIZE; i++)
		host_imageInput[i] = image[i];

	// declare GPU memory pointers
	unsigned char *d_in, *d_out;

	// allocate GPU memory
	cudaMalloc((void **)&d_in, ARRAY_BYTES);
	cudaMalloc((void **)&d_out, ARRAY_BYTES);

	cudaMemcpy(d_in, host_imageInput, ARRAY_BYTES, cudaMemcpyHostToDevice);

	// launch the kernel
	boxBlur<<<height, width>>>(d_out, d_in, width, height);

	// copy back the result array to the CPU
	cudaMemcpy(host_imageOutput, d_out, ARRAY_BYTES, cudaMemcpyDeviceToHost);

	encError = lodepng_encode32_file(newFileName, host_imageOutput, width, height);
	if (encError)
		printf("error %u: %s\n", error, lodepng_error_text(encError));

	printf("Image width = %d and height = %d.\nOutput image name = %s\n", width, height, newFileName);
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}

__global__ void boxBlur(unsigned char *gpu_imageOuput, unsigned char *gpu_imageInput, int width, int height)
{
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int pixel = idx * 4;

	int i = pixel;
	int row = 4 * blockDim.x;
	int full = row * gridDim.x;

	unsigned int r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0;

	if (i == 0) //Left topmost corner pixel
	{
		r1 = i, r2 = i + 4, r3 = row, r4 = row + 4;
		RGB_SumDivide(i, 4, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i == (row - 4)) //(i==4)  Right topmost corner pixel
	{
		r1 = i - 4, r2 = i, r3 = row * 2 - 4 - 4, r4 = row * 2 - 4;
		RGB_SumDivide(i, 4, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i == (row * height) - row) //(i==20)  Left bottom-most corner pixel
	{
		r1 = full - (row * 2), r2 = full - (row * 2) + 4, r3 = i, r4 = i + 4;
		RGB_SumDivide(i, 4, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i == (row * height - 4)) //i==24  Right bottom-most corner pixel
	{
		r1 = full - (row * 2) - (4 * 2), r2 = full - (row * 2) - 4, r3 = i - 4, r4 = i;
		RGB_SumDivide(i, 4, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i < (row)) // Top Edge Row
	{
		r1 = i - 4, r2 = i, r3 = i + 4, r4 = row + (i - 4), r5 = row + i, r6 = row + i + 4;
		RGB_SumDivide(i, 6, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i == (row * (int)(i / row))) // Left Edge Row
	{
		r1 = i - row, r2 = i - row + 4, r3 = i, r4 = i + 4, r5 = i + row, r6 = i + row + 4;
		RGB_SumDivide(i, 6, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}

	else if (i == row * (int)((i + row) / row) - 4) // Rigth Edge Row
	{
		r1 = i - row - 4, r2 = i - row, r3 = i - 4, r4 = i, r5 = i + row - 4, r6 = i + row;
		RGB_SumDivide(i, 6, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else if (i > (row * height) - row) // Bottom Edge Row
	{
		r1 = i - row - 4, r2 = i - row, r3 = i - row + 4, r4 = i - 4, r5 = i, r6 = i + 4;
		RGB_SumDivide(i, 6, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
	else // All Center Pixels
	{
		r1 = i - row + 4, r2 = i + 4 - row + 4, r3 = i + 4 + 4 - row + 4, r4 = i - 4, r5 = i, r6 = i + 4, r7 = i + row - 4, r8 = i + row, r9 = i + row + 4;
		RGB_SumDivide(i, 9, gpu_imageOuput, gpu_imageInput, r1, r2, r3, r4, r5, r6, r7, r8, r9);
	}
}

__device__ void RGB_SumDivide(int j, int d, unsigned char *gpu_imageOuput, unsigned char *gpu_imageInput, int r1, int r2, int r3, int r4, int r5, int r6, int r7, int r8, int r9)
{
	unsigned int r = 0, g = 0, b = 0, t = 0;
	if (d == 4)
	{
		r = gpu_imageInput[r1] + gpu_imageInput[r2] + gpu_imageInput[r3] + gpu_imageInput[r4];
		g = gpu_imageInput[r1 + 1] + gpu_imageInput[r2 + 1] + gpu_imageInput[r3 + 1] + gpu_imageInput[r4 + 1];
		b = gpu_imageInput[r1 + 2] + gpu_imageInput[r2 + 2] + gpu_imageInput[r3 + 2] + gpu_imageInput[r4 + 2];
	}
	else if (d == 6)
	{
		r = gpu_imageInput[r1] + gpu_imageInput[r2] + gpu_imageInput[r3] + gpu_imageInput[r4] + gpu_imageInput[r5] + gpu_imageInput[r6];
		g = gpu_imageInput[r1 + 1] + gpu_imageInput[r2 + 1] + gpu_imageInput[r3 + 1] + gpu_imageInput[r4 + 1] + gpu_imageInput[r5 + 1] + gpu_imageInput[r6 + 1];
		b = gpu_imageInput[r1 + 2] + gpu_imageInput[r2 + 2] + gpu_imageInput[r3 + 2] + gpu_imageInput[r4 + 2] + gpu_imageInput[r5 + 2] + gpu_imageInput[r6 + 2];
	}
	else if (d == 9)
	{
		r = gpu_imageInput[r1] + gpu_imageInput[r2] + gpu_imageInput[r3] + gpu_imageInput[r4] + gpu_imageInput[r5] + gpu_imageInput[r6] + gpu_imageInput[r7] + gpu_imageInput[r8] + gpu_imageInput[r9];
		g = gpu_imageInput[r1 + 1] + gpu_imageInput[r2 + 1] + gpu_imageInput[r3 + 1] + gpu_imageInput[r4 + 1] + gpu_imageInput[r5 + 1] + gpu_imageInput[r6 + 1] + gpu_imageInput[r7 + 1] + gpu_imageInput[r8 + 1] + gpu_imageInput[r9 + 1];
		b = gpu_imageInput[r1 + 2] + gpu_imageInput[r2 + 2] + gpu_imageInput[r3 + 2] + gpu_imageInput[r4 + 2] + gpu_imageInput[r5 + 2] + gpu_imageInput[r6 + 2] + gpu_imageInput[r7 + 2] + gpu_imageInput[r8 + 2] + gpu_imageInput[r9 + 2];
	}

	t = gpu_imageInput[j + 3];
	gpu_imageOuput[j] = r / d;
	gpu_imageOuput[j + 1] = g / d;
	gpu_imageOuput[j + 2] = b / d;
	gpu_imageOuput[j + 3] = t;
}
