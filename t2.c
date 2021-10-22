#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

int count = 0; // A counter used to track the number of combinations explored so far
pthread_mutex_t mutex, mutex1;
int check = 0;

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void *crack(void *salt_and_encrypted)
{

    int x, y, z;   // Loop counters
    char salt[7];  // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
    char plain[7]; // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
    char *enc;     // Pointer to the encrypted password

    char *abc = (char *)salt_and_encrypted;
    substr(salt, abc, 0, 6);
    for (x = 'A'; x <= 'Z'; x++)
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
                printf("%c%c%d %ld \n", x, y, z, pthread_self());


                pthread_mutex_lock(&mutex);
                enc = (char *)crypt(plain, salt);
                count++;

                if (strcmp(salt_and_encrypted, enc) == 0)
                {
                    printf("#%-8d%s %s\n", count, plain, enc);
                    check = 1;
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    }
}

int main(int argc, char *argv[])
{

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
    pthread_t thread[3];
    char name[100] = "$6$AS$9IwGTn5WbHSalUs4ba3JbOfOUX/v1yD71Z4M2F6Yusz5k2WQEOFxqLIY80tudGtcFttqr/Zq6RIPjHkl/t2Pp1";
    pthread_create(&thread[0], NULL, crack, &name);
    pthread_create(&thread[1], NULL, crack, &name);
    pthread_create(&thread[2], NULL, crack, &name);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    printf("%d solutions explored\n", count);

    return 0;
}
