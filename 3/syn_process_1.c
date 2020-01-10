#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <sys/sem.h>
#include <sys/ipc.h>


int main()
{
	int i;

	struct sembuf up= {0, 1, 0};
	struct sembuf down= {0, -1, 0};
	
	int semaphore;
	semaphore=semget(IPC_PRIVATE,1,0600);

	if (fork())
	{
		semop(semaphore,&up,1);
		for (i=0;i<10;i++){
			semop(semaphore,&down,1);
			display("Hello world\n");
			semop(semaphore,&up,1);
		}
		wait(NULL);
	}
	else
	{
		for (i=0;i<10;i++){
			semop(semaphore,&down,1);
			display("Kalimera kosme\n");
			semop(semaphore,&up,1);
		}
	}
	semaphore=semctl(semaphore, 0, IPC_RMID);
	return 0;
}
