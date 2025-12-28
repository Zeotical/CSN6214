#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int buffer[5];
int count = 0;
pthread_mutex_t lock; // Add mutex
void* producer()
{
 for (int i = 0; i < 10; i++) {
 pthread_mutex_lock(&lock); // Lock critical section
 
 if (count < 5) {
 buffer[count] = i;
 printf("PR: Produced: %d\n", i);
 count++;
 printf("PR: Counter: %d\n", count);
 }
 
 pthread_mutex_unlock(&lock); // Unlock critical section
 sleep(1);
 }
 return NULL;
}
void* consumer()
{
 for (int i = 0; i < 10; i++) {
 pthread_mutex_lock(&lock); // Lock critical section
 
 if (count > 0) {
 count--;
 printf("CN: Consumed: %d\n", buffer[count]);
 printf("CN: Counter: %d\n", count);
 }
  
 pthread_mutex_unlock(&lock); // Unlock critical section
 sleep(1);
 }
 return NULL;
}
int main()
{
 pthread_t p, c;
 
 pthread_mutex_init(&lock, NULL); // Initialize mutex
 
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 pthread_join(p, NULL);
 pthread_join(c, NULL);
  pthread_mutex_destroy(&lock); // Destroy mutex
 
 return 0;
}
// Compile: gcc -pthread producer_consumer.c -o pc