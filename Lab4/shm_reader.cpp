#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
using namespace std;
// Define the data structure (must be identical to the writer's definition)
struct SharedData {
int counter;
char buffer[256];
};
int main(int argc, char* argv[]) {
if (argc != 2) {
cerr << "Usage: " << argv[0] << " <SHM_NAME>" << endl;
return 1;
}
const char* SHM_NAME = argv[1];
const size_t SHM_SIZE = 1024;
int shm_fd;
SharedData* shm_ptr = nullptr;
// 1. Open the existing shared memory object
shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
if (shm_fd == -1) {
perror("[READER] shm_open failed");
return 1;
}
// 2. Map the shared memory object into the process's address space (read-only)
shm_ptr = (SharedData*)mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
if (shm_ptr == MAP_FAILED) {
perror("[READER] mmap failed");
return 1;
}
cout << "[READER] Child Process (PID: " << getpid() << ") connected to shared memory." << endl;
// 3. Continuously read until termination signal (-1)
while (shm_ptr->counter != -1) {
if (shm_ptr->counter > 0) {
cout << "[READER] Received Counter: " << shm_ptr->counter
<< " | Message: " << shm_ptr->buffer << endl;
}
usleep(500000); // Wait 0.5 seconds before checking again
}
cout << "[READER] Received termination signal. Exiting." << endl;
// Cleanup: Unmap the memory (unlink is done by the writer/parent)
munmap(shm_ptr, SHM_SIZE);
close(shm_fd); // Close the file descriptor
return 0;
}