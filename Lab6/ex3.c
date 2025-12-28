#include <stdio.h>
#include <sched.h> // Declares sched_getscheduler()
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h> // Required for getpriority() and PRIO_PROCESS
#include <errno.h> // Required for error checking (errno)
#include <string.h> // Required for strerror()
int main() {
 pid_t pid = getpid();
 int policy = sched_getscheduler(pid); 
 printf("Process ID: %d\n", pid);
 printf("Current scheduling policy: ");
 
 switch(policy) {
 case SCHED_FIFO:
 printf("SCHED_FIFO (Real-Time)\n");
 break;
 case SCHED_RR:
 printf("SCHED_RR (Real-Time)\n");
 break;
 case SCHED_OTHER:
 printf("SCHED_OTHER (CFS)\n");
 
 // Correctly retrieve nice value with error check
 errno = 0; 
 int nice_value = getpriority(PRIO_PROCESS, pid);
 if (errno != 0) {
 fprintf(stderr, "Error retrieving nice value: %s\n", 
strerror(errno));
 } else {
 printf("Nice value: %d\n", nice_value);
 }
 break;
 case SCHED_BATCH:
 printf("SCHED_BATCH\n");
 break;
 case SCHED_IDLE:
 printf("SCHED_IDLE\n");
 break;
 default:
 printf("Unknown (%d)\n", policy);
 }
 
 return 0;
}
