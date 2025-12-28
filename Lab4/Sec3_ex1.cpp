#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
// Use the entire std namespace for cleaner I/O
using namespace std;
// Global variable initialized in the passive Program file
int global_var = 100;
int main() {
// Local (stack) variable initialized when the Program becomes a Process
int local_var = 50;
cout << "--- Program State BEFORE Fork (Parent Process " << getpid() << ") ---" << endl;
cout << "Initial Global Variable (global_var): " << global_var << endl;
cout << "Initial Local Variable (local_var): " << local_var << endl;

// Create a new process using the fork() system call
pid_t pid = fork();
if (pid == -1) {
// Error handling
cerr << "fork failed" << endl;
return EXIT_FAILURE;
} else if (pid == 0) {
// --- CHILD PROCESS CODE BLOCK ---
cout << "\n--- Child Process " << getpid() << " (Parent " << getppid() << ") Starting ---" << endl;
// Child modifies its private copy of the variables
global_var = 999;
local_var = 888;
cout << "Child Process: MODIFIED Global Variable (global_var) to: " << global_var << endl;
cout << "Child Process: MODIFIED Local Variable (local_var) to: " << local_var << endl;
cout << "--- Child Process " << getpid() << " Finishing ---" << endl;
return EXIT_SUCCESS; // Child process exits
} else {

// --- PARENT PROCESS CODE BLOCK ---
cout << "\n--- Parent Process " << getpid() << " Waiting for Child " << pid << " ---" << endl;
// Parent waits for the child to finish
wait(NULL);
cout << "\n--- Program State AFTER Child Finished (Parent Process " << getpid() << ") ---" <<
endl;
// Print the variables again, proving they were not changed by the child
cout << "Parent Final Global Variable (global_var): " << global_var << endl;
cout << "Parent Final Local Variable (local_var): " << local_var << endl;
cout << "\n--- Parent Process Exiting ---" << endl;
}
return EXIT_SUCCESS;
}