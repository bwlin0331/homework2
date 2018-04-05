#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

bool *array;//global variable for threads to refer to
void mirror(int n)
{
	int temp = n;
  int rev=0;
  while(n>0)
    {
      rev*=10;
      rev+=n%10;
      n/=10;
    }
	
	if(array[rev] == true){
		printf("%d and %d are reverses\n", temp, rev);
	}
	
  //return rev==m ? true : false;
}
void *sieve(void *n){
    int *num = (int*)n;
		double nm = (double)(*num); 
		nm = sqrt(nm);
   	int i,j;
   	for ( i = 2 ; (double)i < nm ; i++ )
   	{
			if(array[i] == true){
			 for(j = i*i; j < *num; j += i){
					array[j] = false;
				}
			}     	

 		}
 		return; //clean exit of thread
}
void* reverse(void * data) { //second thread of assignment

	printf("\nPrime numbers, digit reversed less than inputed n:\n");
  int i,j; ; 
	int n = *((int*)data);
	
	for ( i = 12; i <= n; i++) {	
		if (array[i] == true) {	
			mirror(i);//prints if reverse exists in list
		}
	}
	return; //exits thread
	
}
int main(int argc, char* argv[]){
	int n = atoi(argv[1]); // enter upper bound at command line
	n++;
	array = (bool*)(malloc((n)*sizeof(bool)));
	int i;
	for(i = 0; i < n; i++){//initialize all to true
		array[i] = true;
	}
	pthread_t tid;
	pthread_create(&tid, NULL, &sieve,&n);
	pthread_join(tid,NULL);
	printf("Prime numbers less than or equal to n: \n");
	for(i = 1; i < n; i++){
			if(array[i] == true){
				printf("%d\n", i);
			}
	}	

	pthread_create(&tid, NULL, &reverse,&n);
	pthread_join(tid,NULL);
}
