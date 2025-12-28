#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define N 5 // Number of philosophers
sem_t chopsticks[N]; // One semaphore per chopstick
// ============================================================
// SOLUTION 1: Simple approach (CAN DEADLOCK!)
// ============================================================
void* philosopher_simple(void* arg) {
 int id = *(int*)arg;
 int left = id;
 int right = (id + 1) % N;
 
 for (int i = 0; i < 3; i++) {
 // Think
 printf("Philosopher %d is thinking\n", id);
 sleep(1);
 
 // Pick up chopsticks
 printf("Philosopher %d is hungry\n", id);
 sem_wait(&chopsticks[left]); // Pick up left chopstick
 printf("Philosopher %d picked up left chopstick %d\n", id, left);
 sem_wait(&chopsticks[right]); // Pick up right chopstick
 printf("Philosopher %d picked up right chopstick %d\n", id, right);
 
 // Eat
 printf("Philosopher %d is EATING\n", id);
 sleep(2);
 
 // Put down chopsticks
 sem_post(&chopsticks[left]);
sem_post(&chopsticks[right]);
 printf("Philosopher %d finished eating\n", id);
 }
 return NULL;
}
// ============================================================
// SOLUTION 2: Asymmetric (Prevents Deadlock)
// ============================================================
void* philosopher_asymmetric(void* arg) {
 int id = *(int*)arg;
 int left = id;
 int right = (id + 1) % N;
 
 for (int i = 0; i < 3; i++) {
 printf("Philosopher %d is thinking\n", id);
 sleep(1);
 
 printf("Philosopher %d is hungry\n", id);
 
 // ASYMMETRIC: Last philosopher picks up in reverse order
 if (id == N - 1) {
 sem_wait(&chopsticks[right]); // Right first
 printf("Philosopher %d picked up right chopstick %d\n", id, 
right);
 sem_wait(&chopsticks[left]); // Then left
 printf("Philosopher %d picked up left chopstick %d\n", id, 
left);
 } else {
 sem_wait(&chopsticks[left]); // Left first
 printf("Philosopher %d picked up left chopstick %d\n", id, 
left);
 sem_wait(&chopsticks[right]); // Then right
 printf("Philosopher %d picked up right chopstick %d\n", id, 
right);
 }
 
 printf("Philosopher %d is EATING\n", id);
 sleep(2);
 
 sem_post(&chopsticks[left]);
 sem_post(&chopsticks[right]);
 printf("Philosopher %d finished eating\n", id);
 }
 return NULL;
}
// ============================================================
// SOLUTION 3: Limit Diners (Prevents Deadlock)
// ============================================================
sem_t room; // Limit philosophers at table
void* philosopher_limited(void* arg) {
 int id = *(int*)arg;
 int left = id;
 int right = (id + 1) % N;
 
 for (int i = 0; i < 3; i++) {
printf("Philosopher %d is thinking\n", id);
 sleep(1);
 
 // Only allow N-1 philosophers at table
 sem_wait(&room);
 printf("Philosopher %d entered dining room\n", id);
 
 printf("Philosopher %d is hungry\n", id);
 sem_wait(&chopsticks[left]);
 printf("Philosopher %d picked up left chopstick %d\n", id, left);
 sem_wait(&chopsticks[right]);
 printf("Philosopher %d picked up right chopstick %d\n", id, right);
 
 printf("Philosopher %d is EATING\n", id);
 sleep(2);
 
 sem_post(&chopsticks[left]);
 sem_post(&chopsticks[right]);
 printf("Philosopher %d finished eating\n", id);
 
 sem_post(&room);
 printf("Philosopher %d left dining room\n", id);
 }
 return NULL;
}
int main() {
 pthread_t philosophers[N];
 int ids[N];
 
 // Initialize chopstick semaphores
 for (int i = 0; i < N; i++) {
 sem_init(&chopsticks[i], 0, 1); // Each chopstick available
 ids[i] = i;
 }
printf("=== Choose Solution ===\n");
 printf("1: Simple (CAN deadlock)\n");
 printf("2: Asymmetric (NO deadlock)\n");
 printf("3: Limited diners (NO deadlock)\n");
 printf("Using Solution 2 (Asymmetric)...\n\n");
 
 // Create philosopher threads
 for (int i = 0; i < N; i++) {
 pthread_create(&philosophers[i], NULL, philosopher_asymmetric, 
&ids[i]);
 }
 
 // Wait for all philosophers
 for (int i = 0; i < N; i++) {
 pthread_join(philosophers[i], NULL);
 }
 
 // Cleanup
 for (int i = 0; i < N; i++) {
 sem_destroy(&chopsticks[i]);
 }
 printf("\nAll philosophers finished dining!\n");
 
 return 0;
}
// Compile: gcc -pthread dining.c -o dining