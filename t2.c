#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>
/*
    While running this task-2 programme you have to pass how many number 
    of threads you want to create and along with encrypted password. If you donot 
    specify like this then the programe will throw "Segmentation fault (core dumped)" error.

    Example:-
        To Compile = gcc t2.c -o xyz -pthread -lcrypt
        To Run = ./xyz [numbers_of_threads] [encrypted_password] 

                 like this ↓↓↓ 
               = ./xyz 3 \$6\$AS\$9IwGTn5WbHSalUs4ba3JbOfOUX/v1yD71Z4M2F6Yusz5k2WQEOFxqLIY80tudGtcFttqr/Zq6RIPjHkl/t2Pp1
               = ./xyz 3 \$6\$AS\$P.Wy8B/NjpVgwGDKZ1uafxVzLNC7UpfX4yBca4BB03TvxHd0hRhjo0.qr1SpHDU2tzOTwTaVB5/8wm8f6Wgcf.
               = (accordingly)

    ## Some of the encrypted password you can try!!
    // AA11 = \$6\$AS\$9IwGTn5WbHSalUs4ba3JbOfOUX/v1yD71Z4M2F6Yusz5k2WQEOFxqLIY80tudGtcFttqr/Zq6RIPjHkl/t2Pp1
    // AA00 = \$6\$AS\$wKDMKDtx/s3ILNkNaRNFIM0w81/weD1UZ8daNhbQBXuj8L.7OY4trHnSraeizmFYrMwjlb1uRTPxu20rqhmMn/
    // MN50 = \$6\$AS\$jOcBmv6TLDDRAKRbpo0EYhYjK.avLXXifvRcbk.RQMllJYfTE/NIS9wm.rkvbIjsA.aMKOA2K5D5IRiDDQEp80
    // ZZ99 = \$6\$AS\$P.Wy8B/NjpVgwGDKZ1uafxVzLNC7UpfX4yBca4BB03TvxHd0hRhjo0.qr1SpHDU2tzOTwTaVB5/8wm8f6Wgcf.

*/

pthread_mutex_t mutex;
char encrypted_password[92] = "";
int threadCount, i, check = 0, alphabet = 26, count = 0;

struct AlphabetVariables
{
    int start;
    int end;
};
void slicing();
void *crack(void *alphabetValue);
void substr(char *dest, char *src, int start, int length);


void main(int argc, char **argv)
{
    // In argument array index 1st how many thread to run 
    // and in 2nd encrypted password of main function.
    threadCount = atoi(argv[1]);
    strcpy(encrypted_password, argv[2]);
    pthread_mutex_init(&mutex, NULL);
    slicing();

    printf("%d solutions explored\n", count);
    pthread_mutex_destroy(&mutex);
}

void slicing()
{
    int sliceList[threadCount],
        remainder = alphabet % threadCount,
        startList[threadCount],
        endList[threadCount];
    struct AlphabetVariables mainStruct[threadCount];

    for (i = 0; i < threadCount; i++)
    {
        sliceList[i] = alphabet / threadCount;
        if (remainder > 0)
        {
            sliceList[i] += 1;
            remainder--;
        }

        if (i == 0)
            startList[i] = 0;
        else
            startList[i] = endList[i - 1] + 1;

        endList[i] = startList[i] + sliceList[i] - 1;

        // Assign to Structure
        mainStruct[i].start = startList[i];
        mainStruct[i].end = endList[i];
    }

    pthread_t thread[threadCount];
    for (i = 0; i < threadCount; i++)
        pthread_create(&thread[i], NULL, crack, &mainStruct[i]);

    for (i = 0; i < threadCount; i++)
        pthread_join(thread[i], NULL);
}

void *crack(void *alphabetValue)
{
    int x, y, z;
    char salt[7];
    char plain[7];
    char *enc;

    struct AlphabetVariables *value = (struct AlphabetVariables *)alphabetValue;
    char startLimit = value->start + 65, endLimit = value->end + 65;

    substr(salt, encrypted_password, 0, 6);
    for (x = startLimit; x <= endLimit; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                if (check)
                {
                    pthread_exit(NULL);
                }
                sprintf(plain, "%c%c%02d", x, y, z);

                pthread_mutex_lock(&mutex);

                count++;
                enc = (char *)crypt(plain, salt);
                if (strcmp(encrypted_password, enc) == 0)
                {
                    printf("#%-8d%s %s\n", count, plain, enc);
                    check = 1;
                }

                pthread_mutex_unlock(&mutex);
            }
        }
    }
}

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}
