#include <stdio.h> // For standard I/O functions (printf, perror)
#include <sys/statvfs.h> // For the statvfs() system call
#include <errno.h> // For error numbers (if statvfs fails)
// Define constants for conversion
#define BYTES_PER_GIGABYTE (1024.0 * 1024.0 * 1024.0)
#define MIN_REQUIRED_GB 1.0 // Minimum required free space (e.g., 1 GB)
/**
* Checks the available disk space for the file system containing the specified path.
* * Uses the statvfs() system call, which is a key OS service for
* "Information Maintenance" and "File-System Manipulation."
*/
int check_disk_availability(const char *path, double min_required_gb) {
 struct statvfs vfs_stat;
 // SYSTEM CALL: statvfs(path, &vfs_stat)
 // This call requests the kernel to fill the vfs_stat structure with
 // file system information for the system where 'path' resides.
 if (statvfs(path, &vfs_stat) == -1) {
 perror("Error calling statvfs system call");
 return -1; // Indicate error
 }
 // Calculate total free space available to the user (non-privileged)
 // f_bfree * f_frsize = Free blocks * Fragment size (bytes per block)
 unsigned long long total_free_bytes = 
 (unsigned long long)vfs_stat.f_bavail * vfs_stat.f_frsize;
 
 // Convert to Gigabytes for easier reading
 double available_gb = total_free_bytes / BYTES_PER_GIGABYTE;
 printf("--- Disk Availability Check ---\n");
 printf("Target Directory: %s\n", path);
 printf("Block Size (bytes): %lu\n", vfs_stat.f_frsize);
 printf("Available Blocks (Non-Superuser): %lu\n", vfs_stat.f_bavail);
 printf("Total Free Space (GB): %.2f GB\n", available_gb);
 printf("Minimum Required (GB): %.2f GB\n", min_required_gb);
 printf("-------------------------------\n");
 // Check if the available space meets the minimum requirement
 if (available_gb >= min_required_gb) {
 printf("Disk check successful: Sufficient space is available for I/O.\n");
 return 0; // Success: Disk is available
 } else {
 printf("Disk check failed: Insufficient space for I/O operations.\n");
return 1; // Failure: Disk is not available
 }
}
int main() {
 // Check the disk space on the file system hosting the current directory (".").
 return check_disk_availability(".", MIN_REQUIRED_GB); 
}