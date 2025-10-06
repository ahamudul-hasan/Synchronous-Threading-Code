#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // lock
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

int turn = 3;

void *threadA(void *arg)
{

    pthread_mutex_lock(&m);
    while (turn != 1)
    {
        pthread_cond_wait(&c, &m);
    }
    printf("This is statement 1\n");
    pthread_mutex_unlock(&m);
    return NULL;
}

void *threadB(void *arg)
{
    pthread_mutex_lock(&m);

    while (turn != 2)
    {
        pthread_cond_wait(&c, &m);
    }
    printf("This is statement 2\n");
    turn = 1;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
    return NULL;
}

void *threadC(void *arg)
{

    printf("This is statement 3\n");

    pthread_mutex_lock(&m);
    turn = 2;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);

    return NULL;
    ;
}

int main()
{
    pthread_t tA, tB, tC;
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tC, NULL, threadC, NULL);
    pthread_create(&tB, NULL, threadB, NULL);

    pthread_join(tA, NULL);
    pthread_join(tC, NULL);
    pthread_join(tB, NULL);
}