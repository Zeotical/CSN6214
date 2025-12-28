#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0; // Number of items in buffer
// Mutex and Condition Variables
pthread_mutex_t mutex;
pthread_cond_t cond_producer; // Signals when space is available
pthread_cond_t cond_consumer; // Signals when items are available
void* producer() { //void* producer(void *arg)
 for (int i = 0; i < 10; i++) {
 pthread_mutex_lock(&mutex);
 
 // Wait while buffer is full
 while (count == BUFFER_SIZE) {
 printf("PR: Buffer full, waiting...\n");
 pthread_cond_wait(&cond_producer, &mutex);
 // Mutex released while waiting, reacquired when woken
 }
 
 // Produce item
 buffer[count] = i;
 printf("PR: Produced %d (count=%d)\n", i, count + 1);
 count++;
 
 // Signal consumer that item is available
 pthread_cond_signal(&cond_consumer);
 
 pthread_mutex_unlock(&mutex);
 sleep(1);}
 return NULL;
}
void* consumer() { //void* consumer(void *arg) {
 for (int i = 0; i < 10; i++) {
 pthread_mutex_lock(&mutex);
 
 // Wait while buffer is empty
 while (count == 0) {
printf("CN: Buffer empty, waiting...\n");
 pthread_cond_wait(&cond_consumer, &mutex);
 // Mutex released while waiting, reacquired when woken
 }
 
 // Consume item
 count--;
 int item = buffer[count];
 printf("CN: Consumed %d (count=%d)\n", item, count);
 
 // Signal producer that space is available
 pthread_cond_signal(&cond_producer);
 
 pthread_mutex_unlock(&mutex);
 sleep(2);
 }
 return NULL;
}
int main() {
 pthread_t p, c;
 
 printf("=== Producer-Consumer using Condition Variables ===\n\n");
 
 // Initialize mutex and condition variables
 pthread_mutex_init(&mutex, NULL);
 pthread_cond_init(&cond_producer, NULL);
 pthread_cond_init(&cond_consumer, NULL);
 
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 
 pthread_join(p, NULL);
 pthread_join(c, NULL);
 
 // Cleanup
 pthread_mutex_destroy(&mutex);
 pthread_cond_destroy(&cond_producer);
 pthread_cond_destroy(&cond_consumer);
 
 printf("\nFinal count: %d\n", count);
 return 0;
}
// Compile: gcc -pthread condition_var.c -o condvar