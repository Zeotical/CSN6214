#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <sys/utsname.h>
using namespace std;
// Function to read a specific field from the /proc/[pid]/status file
string get_proc_status_field(int pid, const string& field_name) {
string path = "/proc/" + to_string(pid) + "/status";
ifstream status_file(path);
string line;
if (!status_file.is_open()) {
return "N/A (Error opening status file)";
}
while (getline(status_file, line)) {
if (line.rfind(field_name, 0) == 0) { // Check if line starts with field_name
// Example line: VmSize: 888 kB
size_t colon_pos = line.find(':');
if (colon_pos != string::npos) {
// Extract and clean up the value part
string value = line.substr(colon_pos + 1);
value.erase(0, value.find_first_not_of(" \t")); // Trim leading whitespace
return value;
}
}
}
return "N/A (Field not found)";
}
int main() {
int pid = getpid();
cout << "--- 🔍 Process Control Block (PCB) Equivalent Data ---" << endl;
cout << " (Data gathered using C++ system calls and /proc filesystem)" << endl;
cout << "--------------------------------------------------------" << endl;
// 1. Process Identification (PID, PPID)
cout << "## Identification" << endl;
cout << " * Process ID (PID): " << pid << endl;
cout << " * Parent PID (PPID): " << getppid() << endl;
cout << " * User ID (UID): " << getuid() << endl;
cout << " * Group ID (GID): " << getgid() << endl;
// Read the current command name from /proc/[pid]/comm
ifstream comm_file("/proc/" + to_string(pid) + "/comm");
string command_name;
getline(comm_file, command_name);
cout << " * Command Name: " << command_name << endl;
// 2. Process State and Priority (Requires reading from /proc)
cout << "\n## State and Priority" << endl;
// State (S for Sleeping, R for Running, etc.)
cout << " * State: " << get_proc_status_field(pid, "State:") << endl;
// Priority (Nice value)
int nice_value = getpriority(PRIO_PROCESS, 0);
cout << " * Priority (Nice Value): " << nice_value << endl;
// 3. CPU Usage and Time
cout << "\n## CPU Time" << endl;
struct tms time_buf;
if (times(&time_buf) != (clock_t)-1) {
long clk_tck = sysconf(_SC_CLK_TCK); // Ticks per second
cout << " * User CPU Time (ticks): " << time_buf.tms_utime << endl;
cout << " * System CPU Time (ticks): " << time_buf.tms_stime << endl;
} else {
cout << " * CPU Time: N/A (times() failed)" << endl;
}
// 4. Memory Management Information (from /proc/[pid]/status)
cout << "\n## Memory Management" << endl;
cout << " * Virtual Memory Size (VmSize): " << get_proc_status_field(pid, "VmSize:") << endl;
cout << " * Resident Set Size (VmRSS): " << get_proc_status_field(pid, "VmRSS:") << endl;
// 5. Context / Open Files (limited view)
cout << "\n## Context / Open Files (Partial)" << endl;
// Standard File Descriptors
cout << " * Standard Input FD: " << STDIN_FILENO << endl;
cout << " * Standard Output FD: " << STDOUT_FILENO << endl;
cout << "--------------------------------------------------------" << endl;
return 0;
}
