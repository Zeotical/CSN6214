#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
// Global Mutex Locks (Resources)
pthread_mutex_t first_mutex; // Assume Order = 1
pthread_mutex_t second_mutex; // Assume Order = 5
// Function for the first thread (Process 1) - SAFE
void *do_work_one_SAFE(void *param)
{
 printf("Thread 1: Trying to acquire locks in ORDER 1 -> 5...\n");
 // 1. Acquire first_mutex (Order 1)
 pthread_mutex_lock(&first_mutex);
 printf("Thread 1: Acquired first_mutex. Now acquiring second_mutex...\n");
 // 2. Acquire second_mutex (Order 5)
 pthread_mutex_lock(&second_mutex);
 // CRITICAL SECTION
 printf("Thread 1: Acquired both locks and is doing work.\n");
 // Release locks in reverse order of acquisition
 pthread_mutex_unlock(&second_mutex);
 pthread_mutex_unlock(&first_mutex);
 printf("Thread 1: Finished and released both locks.\n");
 pthread_exit(0);
}
// Function for the second thread (Process 2) - SAFE
void *do_work_two_SAFE(void *param)
{
 printf("Thread 2: Trying to acquire locks in ORDER 1 -> 5...\n");
 // 1. Acquire first_mutex (Order 1) - ***CHANGE FROM DEADLOCKED VERSION***
 pthread_mutex_lock(&first_mutex);
 printf("Thread 2: Acquired first_mutex. Now acquiring second_mutex...\n");
 // 2. Acquire second_mutex (Order 5)
 pthread_mutex_lock(&second_mutex);
 // CRITICAL SECTION
 printf("Thread 2: Acquired both locks and is doing work.\n");
 // Release locks
 pthread_mutex_unlock(&second_mutex);
 pthread_mutex_unlock(&first_mutex);
 printf("Thread 2: Finished and released both locks.\n");
 pthread_exit(0);
}
int main()
{
 pthread_t tid1, tid2;
 // Initialize the mutexes
 pthread_mutex_init(&first_mutex, NULL);
 pthread_mutex_init(&second_mutex, NULL);
 printf("System initialized. Creating SAFE threads...\n");
 // Create the two threads, both running the SAFE functions
 pthread_create(&tid1, NULL, do_work_one_SAFE, NULL);
 pthread_create(&tid2, NULL, do_work_two_SAFE, NULL);
 // Wait for the threads to finish
 pthread_join(tid1, NULL);
 pthread_join(tid2, NULL);
 printf("Main thread: All threads completed successfully (Deadlock Avoided).\n");
 // Destroy mutexes
 pthread_mutex_destroy(&first_mutex);
 pthread_mutex_destroy(&second_mutex);
 return 0;
}