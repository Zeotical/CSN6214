#include <stdio.h> // Includes the C Standard Library API
int main() {
 FILE *file_ptr;
 
 // API Call: fopen() - internally uses the 'open' system call
 file_ptr = fopen("output_api.txt", "w"); 
 if (file_ptr == NULL) {
 perror("Error opening file via API");
 return 1;
 }
 // API Call: fprintf() - internally uses the 'write' system call
 fprintf(file_ptr, "Hello, C API!\n");
 // API Call: fclose() - internally uses the 'close' system call
 fclose(file_ptr);
 printf("API example completed. Check output_api.txt\n");
 return 0;
}