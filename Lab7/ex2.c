#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
// Shared variables for Peterson's Algorithm
bool flag[2] = {false, false}; // Interest of each thread
int turn = 0; // Whose turn is it
int counter = 0; // Shared resource
// Thread 0
void* process0(void* arg) {
 for (int i = 0; i < 5; i++) {
 // Entry section (Peterson's Algorithm)
 flag[0] = true; // I want to enter
 turn = 1; // Give priority to other thread
 while (flag[1] && turn == 1) {
 // Busy wait
 }
 
 // ===== CRITICAL SECTION =====
 printf("Thread 0: Entering critical section\n");
 counter++;
 printf("Thread 0: Counter = %d\n", counter);
 sleep(1);
 printf("Thread 0: Exiting critical section\n");
 // ============================
 
 // Exit section
 flag[0] = false; // I'm done
 
 sleep(1); // Remainder section
 }
 return NULL;
}
// Thread 1
void* process1(void* arg) {
 for (int i = 0; i < 5; i++) {
 // Entry section (Peterson's Algorithm)
 flag[1] = true; // I want to enter
 turn = 0; // Give priority to other thread
 while (flag[0] && turn == 0) {
 // Busy wait
 }
 
 // ===== CRITICAL SECTION =====
 printf("Thread 1: Entering critical section\n");
 counter++;
 printf("Thread 1: Counter = %d\n", counter);
 sleep(1);
 printf("Thread 1: Exiting critical section\n");
 // ============================
 
 // Exit section
 flag[1] = false; // I'm done
 
 sleep(1); // Remainder section
 }
 return NULL;
}

int main() {
 pthread_t t0, t1;
 
 printf("=== Peterson's Algorithm Demo ===\n\n");
 
 pthread_create(&t0, NULL, process0, NULL);
 pthread_create(&t1, NULL, process1, NULL);
 
 pthread_join(t0, NULL);
 pthread_join(t1, NULL);
 
 printf("\nFinal counter value: %d (Expected: 10)\n", counter);
 
 return 0;
}
// Compile: gcc -pthread peterson.c -o peterson