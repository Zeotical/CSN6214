#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int x = 10;
void* doTask1(void* args)
{
 usleep(50000); // sleep for 50000 micro seconds
 printf("Thread 1: %d\n", getpid());
 x++;
 printf("Thread 1: x = %d\n", x);
}
void* doTask2(void* args)
{
 printf("Thread 2: %d\n", getpid());
 x++;
 printf("Thread 2: x = %d\n", x);
}
int main()
{
 // declare threads
 pthread_t t1, t2;
 // create the threads
 pthread_create(&t1, NULL, doTask1, (void*)&x);
 pthread_create(&t2, NULL, doTask2, (void*)&x);
 // wait for the threads to complete
 pthread_join(t1, NULL);
 pthread_join(t2, NULL);
 return 0;
}