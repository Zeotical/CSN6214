#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int x = 10;
int main() {
 printf("in the parent only\n");
 pid_t pid = fork();
 if (pid == 0) {
 // child
 x++;
 printf("[Child Process] - PID =%d\n", getpid());
 printf("child -> x = %d\n", x);
 } else {
 x--;
 printf("[Parent Process] - PID =%d\n", getpid());
 printf("parent -> x = %d\n", x);
 wait(NULL); // parent waits for child to finish
 }
 return 0;
}