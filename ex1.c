#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
// 1. Initialization of the two mutex locks (Resources)
pthread_mutex_t first_mutex; // Resource 1
pthread_mutex_t second_mutex; // Resource 2
// Function for the first thread (Process 1)
void *do_work_one(void *param)
{
 printf("Thread 1: Trying to acquire first_mutex...\n");
 // Acquire first_mutex (Resource 1)
 pthread_mutex_lock(&first_mutex);
 printf("Thread 1: Acquired first_mutex. Now trying to acquire second_mutex...\n");
 // Pause briefly to ensure Thread 2 can acquire its first lock (second_mutex)
 // This timing is what creates the deadlock condition
 sleep(1);
 // Attempt to acquire second_mutex (Resource 2). This is where it will block if Thread 2 holds it.
 pthread_mutex_lock(&second_mutex);
 // CRITICAL SECTION (Will only be reached if deadlock is avoided)
 printf("Thread 1: Acquired both locks. Doing work...\n");
 // Release locks in reverse order of acquisition
 pthread_mutex_unlock(&second_mutex);
 pthread_mutex_unlock(&first_mutex);
 printf("Thread 1: Finished and released both locks.\n");
 pthread_exit(0);
}
// Function for the second thread (Process 2)
void *do_work_two(void *param)
{
printf("Thread 2: Trying to acquire second_mutex...\n");
 // Acquire second_mutex (Resource 2) in the REVERSE ORDER
 pthread_mutex_lock(&second_mutex);
 printf("Thread 2: Acquired second_mutex. Now trying to acquire first_mutex...\n");
 // Pause briefly (not strictly necessary but aids in demonstrating the deadlock)
 sleep(1);
 // Attempt to acquire first_mutex (Resource 1). This is where it will block if Thread 1 holds it.
 pthread_mutex_lock(&first_mutex);
 // CRITICAL SECTION (Will only be reached if deadlock is avoided)
 printf("Thread 2: Acquired both locks. Doing work...\n");
 // Release locks
 pthread_mutex_unlock(&first_mutex);
 pthread_mutex_unlock(&second_mutex);
printf("Thread 2: Finished and released both locks.\n");
 pthread_exit(0);
}
int main()
{
 pthread_t tid1, tid2;
 // Initialize the mutexes (Resources)
 pthread_mutex_init(&first_mutex, NULL);
 pthread_mutex_init(&second_mutex, NULL);
 printf("System initialized. Creating threads...\n");
 // Create the two threads (Processes)
 pthread_create(&tid1, NULL, do_work_one, NULL);
 pthread_create(&tid2, NULL, do_work_two, NULL);
 // Wait for the threads to finish (which they won't if a deadlock occurs)
 pthread_join(tid1, NULL);
 pthread_join(tid2, NULL);
 printf("Main thread: All threads completed (This line is often not reached when deadlocked).\n");
 // Destroy mutexes
 pthread_mutex_destroy(&first_mutex);
 pthread_mutex_destroy(&second_mutex);
 return 0;
}
