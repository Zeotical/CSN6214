#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
int buffer[5];
int count = 0;
// Binary Semaphores for strict alternation
sem_t producer_turn; // 1 = producer can go
sem_t consumer_turn; // 1 = consumer can go
void* producer() {
 for (int i = 0; i < 10; i++) {
 sem_wait(&producer_turn); // Wait for my turn
 
 // Critical Section
 if (count < 5) {
 buffer[count] = i;
 printf("PR: Produced: %d (count=%d)\n", i, count + 1);
 count++;
 }
 
 sem_post(&consumer_turn); // Give turn to consumer
 sleep(1);
 }
 return NULL;
}
void* consumer() {
 for (int i = 0; i < 10; i++) {
 sem_wait(&consumer_turn); // Wait for my turn
 
 // Critical Section
 if (count > 0) {
 count--;
 printf("CN: Consumed: %d (count=%d)\n", buffer[count], count);
 }
 
 sem_post(&producer_turn); // Give turn to producer
 sleep(1);
 }
 return NULL;
}
int main() {
 pthread_t p, c;
 
 printf("=== Binary Semaphores - Strict Alternation ===\n\n");
  // Initialize: Producer goes first
 sem_init(&producer_turn, 0, 1); // Producer can start (1)
 sem_init(&consumer_turn, 0, 0); // Consumer must wait (0)
 
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 
 pthread_join(p, NULL);
 pthread_join(c, NULL);
 
 sem_destroy(&producer_turn);
 sem_destroy(&consumer_turn);
 
 printf("\nFinal count: %d\n", count);
 
 return 0;
}