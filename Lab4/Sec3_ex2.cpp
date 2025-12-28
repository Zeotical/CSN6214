#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;
// 1. Data Segment: Initialized Global Variables
// Stored in the .data segment
int global_initialized = 100;
// 2. BSS Segment: Uninitialized Global Variables (often near .data)
// Stored in the .bss segment (Block Started by Symbol)
int global_uninitialized;
// A function defined here to get an address in the Text/Code Segment
void print_layout_info() {
cout << "--- Current Process Memory Layout (PID: " << getpid() << ") ---" << endl;
}
int main(int argc, char* argv[]) {
// 3. Stack Segment: Local Variables
// Stored on the function call stack
int local_stack_var = 50;
int another_local_stack_var = 60; // Should have a lower address than the previous one
// 4. Heap Segment: Dynamically Allocated Memory
// Managed by new/delete
int* heap_ptr_1 = new int(200);
int* heap_ptr_2 = new int(300);
// --- Printing Memory Addresses ---
cout << "\n## Code/Text Segment (Highest Addresses)" << endl;
// Address of a function (the executable instructions)
cout << "Address of function (print_layout_info): " << (void*)print_layout_info << endl;
cout << "Address of main function: " << (void*)main << endl;
// ---
cout << "\n## Data/BSS Segments (Static/Global Variables)" << endl;
// Addresses of global variables (static data)
cout << "Address of Initialized Global (data): " << &global_initialized << endl;
cout << "Address of Uninitialized Global (bss): " << &global_uninitialized << endl;
// ---
cout << "\n## Heap Segment (Dynamic Memory)" << endl;
// Addresses of dynamically allocated memory (grows up toward the stack)
// Note: Addresses should be above the static/global data segments
cout << "Address of Heap Allocated 1: " << heap_ptr_1 << " (Should be growing UP)" << endl;
cout << "Address of Heap Allocated 2: " << heap_ptr_2 << endl;
// ---
cout << "\n## Stack Segment (Local Variables) (Lowest Addresses)" << endl;
// Addresses of local variables (grows down toward the heap)
// Note: These addresses are typically much lower than the Code/Data segments.
cout << "Address of Local Stack Var 1: " << &local_stack_var << " (Should be growing DOWN)"
<< endl;
cout << "Address of Local Stack Var 2: " << &another_local_stack_var << endl;
// Clean up dynamic memory
delete heap_ptr_1;
delete heap_ptr_2;
cout << "\n--- Execution complete. ---" << endl;
return EXIT_SUCCESS;
}