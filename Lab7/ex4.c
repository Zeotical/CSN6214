#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0; // Index where producer puts item
int out = 0; // Index where consumer takes item
// Semaphores
sem_t empty; // Count of empty slots (initially BUFFER_SIZE)
sem_t full; // Count of full slots (initially 0)
sem_t mutex; // Binary semaphore for mutual exclusion (initially 1)
void* producer() {
 for (int i = 0; i < 10; i++) {
 // Produce an item
 int item = i;
 
 sem_wait(&empty); // Wait if buffer is full (decrement empty)
 sem_wait(&mutex); // Enter critical section
 
 // Add item to buffer
 buffer[in] = item;
 printf("PR: Produced %d at index %d\n", item, in);
 in = (in + 1) % BUFFER_SIZE; // Circular buffer
 
 sem_post(&mutex); // Exit critical section
 sem_post(&full); // Increment count of full slots
 
 sleep(1);
 }
 return NULL;
}
void* consumer() {
 for (int i = 0; i < 10; i++) {
 sem_wait(&full); // Wait if buffer is empty (decrement full)
 sem_wait(&mutex); // Enter critical section
 // Remove item from buffer
 int item = buffer[out];
 printf("CN: Consumed %d from index %d\n", item, out);
 out = (out + 1) % BUFFER_SIZE; // Circular buffer
 
 sem_post(&mutex); // Exit critical section
 sem_post(&empty); // Increment count of empty slots
 
 sleep(2);
 }
 return NULL;
}
int main() {
 pthread_t p, c;
 
 printf("=== Producer-Consumer using Semaphores ===\n\n");
 
 // Initialize semaphores
 sem_init(&empty, 0, BUFFER_SIZE); // 5 empty slots initially
 sem_init(&full, 0, 0); // 0 full slots initially
 sem_init(&mutex, 0, 1); // Binary semaphore (unlocked)
 
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 
 pthread_join(p, NULL);
 pthread_join(c, NULL);
 
 // Destroy semaphores
 sem_destroy(&empty);
 sem_destroy(&full);
 sem_destroy(&mutex);
 
 printf("\nProduction and consumption completed!\n");
 
 return 0;
}
// Compile: gcc -pthread semaphore.c -o semaphore