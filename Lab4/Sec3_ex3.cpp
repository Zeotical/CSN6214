#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
using namespace std;
int main() {
cout << "--- Parent Process (PID: " << getpid() << ") Starting ---" << endl;
// 1. Process State Transition: NEW -> READY/RUNNING
pid_t pid = fork();
if (pid == -1) {
perror("fork failed");
return EXIT_FAILURE;
}
if (pid == 0) {
// --- CHILD PROCESS CODE BLOCK ---
cout << "--- Child Process (PID: " << getpid() << ") is now RUNNING. ---" << endl;
// 2. Process State Transition: RUNNING -> WAITING (BLOCKED)
// We simulate I/O or an event wait by sleeping for 10 seconds.
// During this time, the process is BLOCKED/SLEEPING (State 'S').
cout << "Child will now SLEEP (BLOCK itself) for 10 seconds. Observe its state ('S')." << endl;
sleep(10);
cout << "Child woke up. Exiting now..." << endl;
return 0; // Child exits, enters ZOMBIE state
} else {
// --- PARENT PROCESS CODE BLOCK ---
cout << "Child Process created with PID: " << pid << endl;
// Give the child time to start and enter the sleep state
cout << "Parent waiting 2 seconds before checking child's state..." << endl;
sleep(2);
// 3. Process State Observation: CHILD in BLOCKED/SLEEPING (S)
cout << "\n==========================================================" << endl;
cout << "Check Child State (PID: " << pid << ") NOW with 'ps aux | grep " << pid << "'" << endl;
cout << "The state should be 'S' (Sleeping/Blocked)." << endl;
cout << "==========================================================" << endl;
// Wait for the child to finish (10s sleep + startup time)
waitpid(pid, NULL, 0);
// 4. Process State Observation: TERMINATED -> ZOMBIE (Z)
// The child has terminated, but the parent hasn't collected its status yet.
cout << "\n==========================================================" << endl;
cout << "Child has TERMINATED! Check its ZOMBIE State ('Z') NOW:" << endl;
cout << " Run: 'ps aux | grep " << pid << "'" << endl;
cout << "You have 5 seconds before the parent cleans it up (wait(NULL))." << endl;
cout << "==========================================================" << endl;
// Hold the zombie state open for 5 seconds
sleep(5);
cout << "\nParent exiting. The operating system will reclaim all resources." << endl;
}
return 0;
}