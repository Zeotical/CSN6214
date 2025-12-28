#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int x = 10;
int main() {
 int pipfd[2];
 // create pipe
 if (pipe(pipfd) == -1){
 perror("pipe");
 exit(EXIT_FAILURE);
 }
 
 printf("in the parent only\n");
 pid_t pid = fork();
 if (pid == 0) { // child
 x++;
 printf("[Child Process] - PID =%d\n", getpid());
 printf("child -> x = %d\n", x);
 // child sends the value of x to the parent
 close(pipfd[0]);
 write(pipfd[1], &x, sizeof(int));
 close(pipfd[1]);
 } else { // the parent reads x from the pipe
 close(pipfd[1]);
 read(pipfd[0], &x, sizeof(int));
 x--;
 printf("[Parent Process] - PID =%d\n", getpid());
 printf("parent -> x = %d\n", x);
 close(pipfd[0]);
 wait(NULL); // guarantees the child execution before the parent
 }
 return 0;
}