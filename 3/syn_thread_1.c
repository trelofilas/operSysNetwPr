#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>

void *displayWorld(void *param);
void *displayKosmos(void *param);

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int main()
{
	pthread_t threadWorld,threadKosmos;
	pthread_attr_t attrThread;
	int x=42;

	pthread_mutexattr_t attrMutex;
	pthread_mutexattr_init(&attrMutex);
	pthread_mutex_init(&mutex,&attrMutex);

	pthread_attr_init(&attrThread);
	pthread_create(&threadWorld,&attrThread,displayWorld,(void *)&x);
	pthread_create(&threadKosmos,&attrThread,displayKosmos,(void *)&x);
	pthread_join(threadWorld,NULL);
	pthread_join(threadKosmos,NULL);

	pthread_attr_destroy(&attrThread);
	pthread_mutexattr_destroy(&attrMutex);
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
}

void *displayWorld(void *param){
	int *p=(int *)param;
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		display("Hello world\n");
		pthread_mutex_unlock(&mutex);
	}

}
void *displayKosmos(void *param){
	int *p=(int *)param;
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		display("Kalimera kosme\n");
		pthread_mutex_unlock(&mutex);
	}
}
