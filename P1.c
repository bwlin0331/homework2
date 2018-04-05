#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

long* array;

void *fib(void *n){
 
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
 
}
int main( int argc, char* argv[]) {

	
    int n = atoi(argv[1]);
		//exact size needed
    array = (long*)malloc((n)*sizeof(long));
 
    printf("First %d terms of Fibonacci series are :-\n",n);
   
    pthread_t tid;
		//printf("hello\n");
    pthread_create(&tid,NULL,&fib,&n);
    pthread_join(tid,NULL);
		
    int i; 
    for(i=0;i<n;i++){
        printf("%d\n",array[i]);
    }            
}
