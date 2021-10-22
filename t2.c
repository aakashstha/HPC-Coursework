#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<crypt.h>
#include<string.h>
#define SALT "$6$AS$"

pthread_mutex_t mutex;
char salt_with_password[92]="";
char charArr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
struct slicingData
{
    char start;
    char end;
};
int check = 0;
int count = 0; // A counter used to track the number of combinations explored so far
/**
 Required by lack of standard function in C.   
*/
void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}
void *crack(void *position)
{
    
    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password
    struct slicingData pos = *(struct slicingData *)position;
    substr(salt, salt_with_password, 0, 6);
    for (x = charArr[pos.start]; x <= charArr[pos.end]; x++)
    {
        
        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                sprintf(plain, "%c%c%02d", x, y, z);
                

                if (check == 1) {
                    pthread_exit(NULL);
                }
            
               
                printf("%s \t %ld\n",plain, pthread_self());

                pthread_mutex_lock(&mutex);

                enc = (char *)crypt(plain, SALT);
                count++;
                if (strcmp(salt_with_password, enc) == 0)
                {
                    
                    printf("password found #%-8d%s %s\n", count, plain, enc);
                    check = 1;
                    //return 0;
                   // pthread_exit(NULL);
                   // return;	//uncomment this line if you want to speed-up the running time, program will find you the cracked password only without exploring all possibilites
                }

                pthread_mutex_unlock(&mutex);
            }
        }
    }
    
}
void main(int *argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    int nThreads = atoi(argv[1]);
    pthread_t threadId[nThreads];
    int division = 25 / nThreads;
    int remainder = 25 % nThreads;
    int start = 0, end = 0;
    struct slicingData s[nThreads];
    char input[92];
    strcpy(salt_with_password,argv[2]);

    for (int i = 0; i < nThreads; i++)
    {
        s[i].start = start;
        s[i].end = end + division;
        if (remainder > 0)
        {
            s[i].end += 1;
            remainder--;
        }
        start = s[i].end + 1;
        end = s[i].end;
    }
    for (int j = 0; j < nThreads; j++)
    {
        pthread_create(&threadId[j], NULL, crack, (void *)&s[j]);
    }
    for (int k = 0; k < nThreads; k++)
    {
        pthread_join(threadId[k], NULL);
    }
}


// AA11 = \$6\$AS\$9IwGTn5WbHSalUs4ba3JbOfOUX/v1yD71Z4M2F6Yusz5k2WQEOFxqLIY80tudGtcFttqr/Zq6RIPjHkl/t2Pp1
