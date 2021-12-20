// To run on Google Colab
%%cu
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime_api.h>

/*
	__global__ --> GPU function which can be launched by many blocks and threads
	__device__ --> GPU function or variables
	__host__ --> CPU function or variables

	While running this task3 on Google Colab just uncomment the first statement written %%cu
	and it will run perfectly fine.


	## some of the encrypted password you can try!
	// AZ00 = CCBDWY2244
	// MW23 = OKNZTV4071
	// PO88 = RNQRLN1634
	// ZZ99 = CXBDWY2745
*/
__device__ char * CudaCrypt(char *rawPassword);
__device__ int compareTwoEncryption(char *encrypted, char *generated);
__global__ void crack(char *alphabet, char *numbers, char *encrypted_password, char *crack_password);

int main(int argc, char **argv)
{
	
	char cpuAlphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	char cpuNumbers[26] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	char *gpuAlphabet, *gpuNumbers;
	char encrypted_password[11] = "CXBDWY2745";
	char *gpuCrack_password;
	char *cpuCracked_password = (char *)malloc(sizeof(char) * 11);
	char *gpuEncrypted_password;

	cudaMalloc((void **)&gpuAlphabet, sizeof(char) * 26);
	cudaMalloc((void **)&gpuNumbers, sizeof(char) * 26);
	cudaMalloc((void **)&gpuEncrypted_password, sizeof(char) * 11);
	cudaMalloc((void **)&gpuCrack_password, sizeof(char) * 11);

	cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);
	cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(char) * 26, cudaMemcpyHostToDevice);
	cudaMemcpy(gpuEncrypted_password, encrypted_password, sizeof(char) * 11, cudaMemcpyHostToDevice);

	crack<<<dim3(26, 26, 1), dim3(10, 10, 1)>>>(gpuAlphabet, gpuNumbers, gpuEncrypted_password, gpuCrack_password);
	cudaDeviceSynchronize();

	cudaMemcpy(cpuCracked_password, gpuCrack_password, sizeof(char) * 11, cudaMemcpyDeviceToHost);

	cudaFree(gpuAlphabet);
	cudaFree(gpuNumbers);
	cudaFree(gpuEncrypted_password);

	printf("PASSWORD CRACKED!!\nReceived encryption was and generated encryption is %s.\nYour password is = %s\n", encrypted_password, cpuCracked_password);

	//printf("%s \n", encrypted_password2);

	return 0;
}

__global__ void crack(char *alphabet, char *numbers, char *encrypted_password, char *crack_password)
{
	char genRawPass[4];

	genRawPass[0] = alphabet[blockIdx.x];
	genRawPass[1] = alphabet[blockIdx.y];

	genRawPass[2] = numbers[threadIdx.x];
	genRawPass[3] = numbers[threadIdx.y];

	char *generateEncryptPassword = CudaCrypt(genRawPass);
	int flag = compareTwoEncryption(encrypted_password, generateEncryptPassword);
	if (flag)
	{
		crack_password[0] = genRawPass[0];
		crack_password[1] = genRawPass[1];

		crack_password[2] = genRawPass[2];
		crack_password[3] = genRawPass[3];
		// printf("PASSWORD CRACKED!!\nReceived encryption was %s and generated encryption is %s.\nYour password is = %s\n", encrypted_password, generateEncryptPassword, genRawPass);
	}

	//printf("%c %c %c %c = %s\n", genRawPass[0], genRawPass[1], genRawPass[2], genRawPass[3], CudaCrypt(genRawPass));
}

// To Encrypt All the Characters and Numbers
__device__ char * CudaCrypt(char *rawPassword)
{
	char *newPassword = (char *)malloc(sizeof(char) * 11);

	newPassword[0] = rawPassword[0] + 2;
	newPassword[1] = rawPassword[0] - 2;
	newPassword[2] = rawPassword[0] + 1;
	newPassword[3] = rawPassword[1] + 3;
	newPassword[4] = rawPassword[1] - 3;
	newPassword[5] = rawPassword[1] - 1;
	newPassword[6] = rawPassword[2] + 2;
	newPassword[7] = rawPassword[2] - 2;
	newPassword[8] = rawPassword[3] + 4;
	newPassword[9] = rawPassword[3] - 4;
	newPassword[10] = '\0';

	for (int i = 0; i < 10; i++)
	{
		if (i >= 0 && i < 6)
		{ //checking all lower case letter limits
			if (newPassword[i] > 90)
			{
				newPassword[i] = (newPassword[i] - 90) + 65;
			}
			else if (newPassword[i] < 65)
			{
				newPassword[i] = (65 - newPassword[i]) + 65;
			}
		}
		else
		{ //checking number section
			if (newPassword[i] > 57)
			{
				newPassword[i] = (newPassword[i] - 57) + 48;
			}
			else if (newPassword[i] < 48)
			{
				newPassword[i] = (48 - newPassword[i]) + 48;
			}
		}
	}
	return newPassword;
}

// To check all the encrypted password match with each other or not
__device__ int compareTwoEncryption(char *encrypted, char *generated)
{
	for (int i = 0; i < 10; i++)
	{
		if (encrypted[i] != generated[i])
			return 0;
	}
	return 1;
}

