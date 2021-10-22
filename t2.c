#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

int count = 0; // A counter used to track the number of combinations explored so far
pthread_mutex_t mutex;
char encrypted_password[92]="";
int threadCount;
int check = 0;
int i,n=26;

struct AlphabetVariables
{
    int start;
    int end;
};

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void *crack(void *alphabetValue)
{

    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password

    struct AlphabetVariables *value = (struct AlphabetVariables *)alphabetValue;
    char startLimit = value->start+65, endLimit = value->end+65;

    substr(salt, encrypted_password, 0, 6);
    for (x = startLimit; x <= endLimit; x++)
    {
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                if (check == 1)
                {
                    pthread_exit(NULL);
                }
                sprintf(plain, "%c%c%02d", x, y, z);
               // printf("%c%c%d %ld \n", x, y, z, pthread_self());


                pthread_mutex_lock(&mutex);
                enc = (char *)crypt(plain, salt);
                count++;

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

void slicing()
{
    int sliceList[threadCount],
        remainder = n % threadCount,
        startList[threadCount],
        endList[threadCount];
    struct AlphabetVariables mainStruct[threadCount];

    for (i = 0; i < threadCount; i++)
    {
        sliceList[i] = n / threadCount;
        if (remainder > 0) {
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
    // for (i = 0; i < threadCount; i++)
    //     printf("%d %d \n",mainStruct[i].start, mainStruct[i].end);

    pthread_t thread[threadCount];
    for (i = 0; i < threadCount; i++)
        pthread_create(&thread[i], NULL, crack, &mainStruct[i]);

    for (i = 0; i < threadCount; i++)
        pthread_join(thread[i], NULL);
}

int main(int argc, char *argv[])
{
   threadCount = atoi(argv[1]);
    strcpy(encrypted_password,argv[2]);
    pthread_mutex_init(&mutex, NULL);
    slicing();
    
    printf("%d solutions explored\n", count);
    return 0;
}