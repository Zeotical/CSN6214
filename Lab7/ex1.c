#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int buffer[5];
int count = 0;
void* producer()
{
 for (int i = 0; i < 10; i++) {
 if (count < 5) {
 buffer[count] = i;
 printf("PR: Produced: %d\n", i);
 count++;
 printf("PR: Counter: %d\n", count);
 }
 //sleep(1);
 }
 return NULL;
}
void* consumer()
{
 for (int i = 0; i < 10; i++) {
 if (count > 0) {
 count--;
 printf("CN: Consumed: %d\n", buffer[count]);
 printf("CN: Produced: %d\n", count);
 }
 //sleep(1);
 }
 return NULL;
}
int main()
{
 pthread_t p, c;
 pthread_create(&p, NULL, producer, NULL);
 pthread_create(&c, NULL, consumer, NULL);
 pthread_join(p, NULL);
 pthread_join(c, NULL);
 return 0;
}