//this problem requires semaphores, which allows a lock based on a count variable
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include "P9.h"

#define NUMCHILD 20
#define NUMTEACHER 4
#define NUMPARENT 7

sem_t teachercount;
sem_t childcount;
pthread_mutex_t lock; //lock for parent looking at children
float R, parentR;
void wait(sem_t *sem)
{
  int n = sem_wait(sem);
  if (n != 0) {
		printf("sem_wait failed");
		exit(0);
	}
	
} 

void signal(sem_t *sem)
{
  int n = sem_post(sem);
  if (n != 0) {
		printf("sem_post failed");
		exit(0);
	}
	
}

int main(int argc, char *argv[]){
	R = 5.0;
	sem_init(&teachercount,0,0);//initialize shared semaphore count
	sem_init(&childcount,0,0);
	if (pthread_mutex_init(&lock, NULL) != 0)//initialize mutex
  {
        printf("\n mutex init has failed\n");
        return 1;
  }
	pthread_t ctid[NUMCHILD];
	pthread_t ttid[NUMTEACHER];
	pthread_t ptid[NUMPARENT];
	int i;
  
	for(i = 0;i < NUMCHILD; i++){
		//printf("hello\n");
		pthread_create(&ctid[i],NULL,&Child,NULL);
	}
	for(i = 0;i < NUMTEACHER; i++){
		pthread_create(&ttid[i],NULL,&Teacher,NULL);
	}
	for(i = 0;i < NUMPARENT; i++){
		pthread_create(&ptid[i],NULL,&Parent,NULL);
		sleep(1);
	}
	//printf("hello\n");
	for(i = 0;i < NUMCHILD; i++){
		pthread_join(ctid[i],NULL);
	}
	for(i = 0;i < NUMTEACHER; i++){
		pthread_join(ttid[i],NULL);
	}
	for(i = 0;i < NUMPARENT; i++){
		pthread_join(ptid[i],NULL);
	}
	//destroy synchronization variables
	sem_destroy(&teachercount);
	sem_destroy(&childcount);
	pthread_mutex_destroy(&lock);
	return 0;
}

void *Teacher(){
	for(;;){
		teacher_enter();
		teach();
		teacher_exit();
		go_home();
	}
}

void *Child(){
	for(;;){
		child_enter();

		learn();
		child_exit();
		go_home();
	}
}

void *Parent(){
	for(;;){
		parent_enter();
		verify_compliance();
		parent_exit();
		go_home();
	}
}

void go_home(){
	printf("%d went home\n",(int)pthread_self());
	pthread_exit(0);
}

//teacher code
void teacher_enter(){
	signal(&teachercount);
	printf("Teacher %d entered\n",(int)pthread_self());
}

void teach(){
	printf("Teacher %d teaching\n",(int)pthread_self());
	sleep(5);//teaches for 5 seconds
}

void teacher_exit(){
	int teachers;
	sem_getvalue(&teachercount, &teachers);
	int children;
	sem_getvalue(&childcount, &children);
	int tleft = teachers - 1; // value of teachers if this teacher leaves
	float ratio;
	if(tleft > 0){
		ratio = (float)children/tleft;
	}else{
		if(children == 0){
			ratio = 0;
		}else{
			ratio = R + 1;
		}
	}
	if(ratio <= R){
		wait(&teachercount);
		printf("Teacher %d exited\n",(int)(pthread_self()));
	}else{
		printf("Teacher %d cannot leave because ratio would be too high\n", (int)(pthread_self()));
		teach();
		teacher_exit();
	}	
}

// child code
void child_enter(){
	signal(&childcount);
	printf("Child %d entered\n",(int)pthread_self());
}

void child_exit(){
	wait(&childcount);
	printf("Child %d exited\n",(int)pthread_self());
}

void learn(){
	printf("Child %d learning\n",(int)pthread_self());
	sleep(5);
}

// parent code
void parent_enter(){
	printf("Parent %u entered.\n", (int) pthread_self());
}

void verify_compliance(){
	pthread_mutex_lock(&lock);//locks so only one parent can verify at a time
	int teachers;
	sem_getvalue(&teachercount, &teachers);
	int children;
	sem_getvalue(&childcount, &children);	
	if(teachers > 0){
		parentR = (float)children/teachers;
	}else{
		parentR = (children == 0) ? 0:(R+1);
	}
	if(parentR <= R){
		printf("Parent %d verified ratio\n", (int) pthread_self());
	}else{
		printf("Parent %d verified ratio has not been met\n",(int)pthread_self());
	}
		
	pthread_mutex_unlock(&lock);//unlocks so now other parents can verify
}

void parent_exit(){
	printf("Parent %d exited\n",(int)pthread_self());
}
