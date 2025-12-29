#include <stdio.h>
#include <stdbool.h>
// Define system parameters
#define P 5 // Number of processes (P0 to P4)
#define R 3 // Number of resource types (A, B, C)
// Global data structures for the Banker's Algorithm (Initial T0 State)
// Note: This uses the initial state *before* P1's request is provisionally granted.
int max[P][R] = {
 {7, 5, 3}, // P0 Max
 {3, 2, 2}, // P1 Max
 {9, 0, 2}, // P2 Max
 {2, 2, 2}, // P3 Max
 {4, 3, 3} // P4 Max
};
int allocation[P][R] = {
 {0, 1, 0}, // P0 Allocation
 {2, 0, 0}, // P1 Allocation
 {3, 0, 2}, // P2 Allocation
 {2, 1, 1}, // P3 Allocation
 {0, 0, 2} // P4 Allocation
};
int available[R] = {3, 3, 2}; // Available resources (A, B, C)
// Need Matrix calculation
int need[P][R];
// Global temporary variables for safety check
int work[R];
bool finish[P];
int safe_sequence[P];
// Function to calculate the Need Matrix
void calculateNeed()
{
 for (int i = 0; i < P; i++)
 {
 for (int j = 0; j < R; j++)
 {
 // Need[i,j] = Max[i,j] - Allocation[i,j]
 need[i][j] = max[i][j] - allocation[i][j];
 }
 }
}

// ----------------------------------------------------------------------
// 2. Safety Algorithm (Page 335)
// ----------------------------------------------------------------------
bool isSafe()
{
 int i, j, count = 0;
 // 1. Initialize Work and Finish
 for (j = 0; j < R; j++)
 {
 work[j] = available[j]; // Work = Available
 }
 for (i = 0; i < P; i++)
 {
 finish[i] = false; // Finish[] = false
 }
 
 // 2. Find an i such that Finish[i] == false AND Need[i] <= Work
 while (count < P)
 {
 bool found = false;
 for (i = 0; i < P; i++)
 {
 if (finish[i] == false)
 {
 // Check if Need[i] <= Work
 int k;
 for (k = 0; k < R; k++)
 {
 if (need[i][k] > work[k])
 {
 break;
 }
 }
 // If Need[i] <= Work is true
 if (k == R)
 {
 // 3. Work = Work + Allocation[i] and Finish[i] = true
 for (j = 0; j < R; j++)
 {
 work[j] += allocation[i][j];
 }
 finish[i] = true;
 safe_sequence[count++] = i;
 found = true;
 }
 }
 }
 // If no such i is found in an iteration, break the loop
 if (found == false)
 {
 break;
 }
 }

// 4. Check if Finish[i] == true for all i
 if (count == P)
 {
 return true; // System is in a safe state
 }
 else
 {
 return false; // System is in an unsafe state
 }
}
// ----------------------------------------------------------------------
// 3. Resource-Request Algorithm (Page 29)
// ----------------------------------------------------------------------
void requestResources(int pid, int request[])
{
 int i, j;
 printf("\nProcess P%d requests resources: (%d, %d, %d)\n",
 pid, request[0], request[1], request[2]);
 // Step 1: Check Request <= Need
 for (i = 0; i < R; i++)
 {
 if (request[i] > need[pid][i])
 {
 printf("--- ERROR: P%d exceeded its maximum claim (Request > Need) ---\n", pid);
 return;
 }
 }
 // Step 2: Check Request <= Available
 for (i = 0; i < R; i++)
 {
 if (request[i] > available[i])
 {
 printf("--- P%d must wait. Resources are not available (Request >Available) ---\n", pid);
 return;
 }
 }
 // Step 3: Pretend to allocate requested resources
 // Save current state for restoration if necessary
 int alloc_temp[R], avail_temp[R], need_temp[R];
 for (i = 0; i < R; i++)
 {
 alloc_temp[i] = allocation[pid][i];
 avail_temp[i] = available[i];
 need_temp[i] = need[pid][i];
 }
 // Apply temporary allocation
 for (i = 0; i < R; i++)
 {
 available[i] -= request[i];
 allocation[pid][i] += request[i];
 need[pid][i] -= request[i];
 }
 // Run Safety Algorithm
 if (isSafe())
 {
 // If safe, the allocation is granted
 printf(">>> Request GRANTED. System remains in a safe state.\n");
 printf(" Safe Sequence found: <");
 for (i = 0; i < P; i++)
 {
 printf("P%d%s", safe_sequence[i], (i == P - 1) ? "" : ", ");
 }
 printf(">\n");
 }
 else
 {
 // If unsafe, restore the old state and P must wait
 printf(">>> Request DENIED. Granting request would lead to an unsafe state.\n");
 printf(" P%d must wait, and the old resource-allocation state is restored.\n", pid);
 for (i = 0; i < R; i++)
 {
 allocation[pid][i] = alloc_temp[i];
 available[i] = avail_temp[i];
 need[pid][i] = need_temp[i];
 }
 }
}

// ----------------------------------------------------------------------
// 4. Main Execution
// ----------------------------------------------------------------------
int main()
{
 calculateNeed(); // Calculate initial Need Matrix
 printf("--- Initial State at T0 ---\n");
 printf("Initial Available: (%d, %d, %d)\n", available[0], available[1], 
available[2]);
 // Check initial state safety (as per the example)
 if (isSafe())
 {
 printf("Initial state is SAFE.\n");
 printf("Safe Sequence found: <");
 for (int i = 0; i < P; i++)
 {
 printf("P%d%s", safe_sequence[i], (i == P - 1) ? "" : ", ");
 }
 printf(">\n");
 }
else
 {
 printf("Initial state is UNSAFE.\n");
 }
 // Example 8.6.3.3: P1 requests (1, 0, 2)
 int request1[] = {1, 0, 2};
 requestResources(1, request1);
 return 0;
}
