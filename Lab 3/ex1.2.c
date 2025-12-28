#include <unistd.h> // Provides access to POSIX operating system API (e.g., read, write, close)
#include <fcntl.h> // Provides file control definitions (e.g., O_CREAT, O_WRONLY)
#include <string.h> // Provides strlen()
#include <stdio.h> // For basic printf() and perror()
int main() {
 int file_descriptor; // System calls use an integer 'file descriptor'
 char *message = "Hello, System Calls!\n";
 ssize_t bytes_written;
 // SYSTEM CALL: open() - opens the file for writing (O_WRONLY) and creates it (O_CREAT)
 // The last argument (0644) is the file permission mask.
 file_descriptor = open("output_syscall.txt", O_WRONLY | O_CREAT, 0644);
 if (file_descriptor < 0) { // On error, open() returns a negative value
 perror("Error opening file via System Call");
 return 1;
 }
 // SYSTEM CALL: write() - writes the message to the file descriptor
 bytes_written = write(file_descriptor, message, strlen(message));
 
 if (bytes_written < 0) { // On error, write() returns a negative value
 perror("Error writing via System Call");
 }
 // SYSTEM CALL: close() - closes the file descriptor
 close(file_descriptor);
 printf("System Call example completed. Check output_syscall.txt\n");
 return 0;

}
