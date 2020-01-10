#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>
#include <stdbool.h>

void *displayAB(void *param);
void *displayCD(void *param);

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var=PTHREAD_COND_INITIALIZER;

int predicateAB=true;
int predicateCD=true;

int main()
{
	int i;

	pthread_t threadAB,threadCD;
	pthread_attr_t attrThread;
	int x=42;

	pthread_mutexattr_t attrMutex;
	pthread_mutexattr_init(&attrMutex);
	pthread_mutex_init(&mutex,&attrMutex);
	pthread_cond_init(&cond_var,NULL);

	pthread_attr_init(&attrThread);
	pthread_create(&threadAB,&attrThread,displayAB,(void *)&x);
	pthread_create(&threadCD,&attrThread,displayCD,(void *)&x);
	pthread_join(threadAB,NULL);
	pthread_join(threadCD,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attrThread);
	pthread_mutexattr_destroy(&attrMutex);
	pthread_cond_destroy(&cond_var);
	pthread_exit(NULL);
}

void *displayAB(void *param){
	int *p=(int *)param;
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		while(predicateAB==false){
			pthread_cond_wait(&cond_var,&mutex);
		}
		predicateAB=false;
		pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex);
		display("ab");
		predicateCD=false;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);	
	}
	return NULL;
}

void *displayCD(void *param){
	int *p=(int *)param;
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		while(predicateCD==true){	
			pthread_cond_wait(&cond_var,&mutex);
		}
		
		predicateCD=true;
		pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex);
		display("cd\n");
		predicateAB=true;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
