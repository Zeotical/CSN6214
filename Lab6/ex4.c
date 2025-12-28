#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
 cpu_set_t mask;
 pid_t pid = getpid();
 
 // Get current CPU affinity
 if (sched_getaffinity(pid, sizeof(mask), &mask) == -1) {
 perror("sched_getaffinity");
 return 1;
 }
 
 printf("Current CPU affinity mask: ");
 for (int i = 0; i < CPU_SETSIZE; i++) { //works w cpp
 if (CPU_ISSET(i, &mask)) {
 printf("%d ", i);
 }
 }
 printf("\n");
 
 // Set affinity to CPU 0 only
 CPU_ZERO(&mask);
 CPU_SET(0, &mask);
 
 if (sched_setaffinity(pid, sizeof(mask), &mask) == -1) {
 perror("sched_setaffinity");
 return 1;
 }
 
 printf("CPU affinity set to CPU 0 only\n");
 
 // Do some work to see the effect
 volatile long sum = 0;
 for (long i = 0; i < 1000000000L; i++) {
 sum += i;
 }
 printf("Computation completed: %ld\n", sum);
 
 return 0;
}