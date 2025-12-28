#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
using namespace std;
const long MAX_MSG_SIZE = 256; // Must match sender's size
// Define the data structure for the message (must be identical)
struct Message {
int counter;
char buffer[252];
};
int main(int argc, char* argv[]) {
if (argc != 2) {
cerr << "Usage: " << argv[0] << " <MQ_NAME>" << endl;
return 1;
}
const char* MQ_NAME = argv[1];
mqd_t mqd;
Message msg;
// 1. Open the existing message queue (read-only)
// Note: We don't need the attributes struct here, as we are not creating it.
mqd = mq_open(MQ_NAME, O_RDONLY);
if (mqd == (mqd_t)-1) {
perror("[RECEIVER] mq_open failed");
return 1;
}
cout << "[RECEIVER] Child Process (PID: " << getpid() << ") connected to message queue." << endl;
// 2. Continuously receive messages until termination signal (-1)
while (true) {
// Receive the message. It blocks until a message is available.
ssize_t bytes_read = mq_receive(mqd, (char*)&msg, MAX_MSG_SIZE, NULL);
if (bytes_read == -1) {
perror("[RECEIVER] mq_receive failed");
break;
}
if (msg.counter == -1) {
cout << "[RECEIVER] Received termination signal. Exiting." << endl;
break;
}
cout << "[RECEIVER] Received Counter: " << msg.counter
<< " | Message: " << msg.buffer << endl;
}
// Cleanup: Close the message queue descriptor
mq_close(mqd);
return 0;
}