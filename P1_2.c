#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

long* array;
volatile int lock = 0;
int testnset(int *lockp){
	int oldv;
	oldv = *lockp;
	*lockp = 1;
	return oldv;
}
void *fib(void *n){
 		//lock = 1;
		printf("%d\n",lock);
     int *num = (int*)n; 
     int  first = 0, second = 1, c;
 		 long next;
   for ( c = 0 ; c < *num ; c++ )
   {
      if ( c <= 1 )
         next = c;
      else
      {
         next = first + second;
         first = second;
         second = next;
      }
      array[c] = next; 
   }
 		lock = 0;
}

int main( int argc, char* argv[]) {

	
    int n = atoi(argv[1]);
		//exact size needed
    array = (long*)malloc((n)*sizeof(long));
 
    printf("First %d terms of Fibonacci series are :-\n",n);
   
    pthread_t tid;
		//printf("hello\n");
    pthread_create(&tid,NULL,&fib,&n);
		lock = 1;
    while(lock == 1){
			//BUSY WAIT
		}
    int i; 
    for(i=0;i<n;i++){
        printf("%d\n",array[i]);
    }  
		lock = 0;          
}
/*The busy wait is necessary because without it, the results printed can be very 
inconsistent. We sometimes that the thread will have done no work or on a subsection
of the array. The issue here is the race conditions between the worker thread and
main thread as they compete to finish their task. Some context switching at random 
points also contribute to the bad array output. The busy wait forces the main thread
to wait for the worker thread to generate all the values in the array before it reads 
and prints from it.

*/
