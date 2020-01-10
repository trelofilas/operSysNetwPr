#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>

#define BUF_SIZE 2048
#define ARG_SIZE 256

void pipesource(int fd[],char *pip1[]);
void pipedestination(int fd[],char *pip1[]);

int main(){
	char buffer[BUF_SIZE];		/*orizetai to string buffer to opoio tha lavei 
    							thn timh ths eisodou apo to xrhsth */
	char *argv[ARG_SIZE];			
	char *path=NULL;
	char *token1;
	char *token2;
	char *token3;

	while(1){
	/* epanalhpsh tou shell mexri o xrhsths na pathsei CTRL+D CTRL+Z h CTRL+C */

		printf("$");								//ektypwnetai to shell prompt		
		if(!fgets(buffer,BUF_SIZE,stdin)){		//lamvanetai h eisodos apo to xrhsth
			exit(0);									//an h eisodos einai lan8asmenh termatismos
		}
		/* antikathistatai o teleutaios xarakthras tou buffer apo newline se NULL */
		size_t length = strlen(buffer);
		if (buffer[length-1]=='\n'){				
			buffer[length-1]='\0';
		}
		/* an h eisodos einai h entolh exit ginetai eksodos apo to programma */
		if(!strcmp(buffer,"exit\0"))
			exit(0);
		/* anazhthsh gia pipes,dhladh gia to sumblo "|" */
		int pipecount=0;
  		char *search=strchr(buffer,'|');
  		while (search!=NULL) {
    		pipecount++;
    		search=strchr(search+1,'|');
  		}
		
		if(pipecount>0){	/* an exei timh >0 eimaste sto mysh[3,4].c
			/* xwrizoume thn eisodo se diaforetikes processes */
			token2=strtok(buffer,"|");
			int j=0;
			while(token2!=NULL){
				argv[j]=token2;
				token2=strtok(NULL,"|");
				j++;
			}
			argv[j]=NULL;
			char *pipeargv[pipecount][ARG_SIZE];


			if(pipecount>1){	/* pipecount>1 (mysh4.c) */
				/*prokeitai gia thn if pou elegxei an 8a exoume panw 
				apo mia diergasia to opoio den ulopoih8hke */
				
				/*diaxwrismos twn arguements ths ka8e process*/
				int t,l=0;
				for(t=0;t<j;t++){
					token3=strtok(argv[t]," \t\n");
					while(token3!=NULL){
						pipeargv[t][l]=token3;
						token3=strtok(NULL," \t\n");
						l++;
					}
				}
			}else{		//pipecount=1(mysh3.c)
				
				/*diaxwrismos twn arguements ths ka8e process*/
				int t,l=0;
				for(t=0;t<j;t++){
					token3=strtok(argv[t]," \t\n");
					while(token3!=NULL){
						pipeargv[t][l]=token3;
						token3=strtok(NULL," \t\n");
						l++;
					}
				}
				/*oi 2 grammes tou disdiastatou pinaka ginontai
				  2 monodiastatoi pinakes */
				char *pipear1[l];
				char *pipear2[l];
				int x;
				for(x=0;x<l;x++){
					pipear1[x]=pipeargv[0][x];
					pipear2[x]=pipeargv[1][x];
				}
				pipear1[2]=NULL;
				pipear2[2]=NULL;

				int pid, status;
				int pfds[2];
				pipe(pfds);

				pipesource(&pfds[0],&pipear1[0]);
				pipedestination(&pfds[0],&pipear2[0]);
				close(pfds[0]);
				close(pfds[1]);
					while ((pid = wait(&status)) != -1){ 
						fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status)); 
					}
					exit(0);
			}
		}else{	//den uparxei pipe(mysh[1,2].c)
		/* xwrizoume thn eisodo sto buffer se ksexwrista arguements */
			token1 = strtok(buffer," \t\n");
			int i=0;
			while(token1!=NULL){
				argv[i]=token1;
				token1 = strtok(NULL," \t\n");
				i++;
			}
			argv[i]=NULL;	//to to teleutaio pairnei thn NULL timh


			if (!strcmp(argv[0],"cd")){				//user press cd the path changes
				path = argv[1];
				chdir(path);
			}else{
				pid_t childpid=fork();			//fork child
				if(childpid==0){			//Child
					execvp(argv[0],argv);	
				}else{						//Parent
					wait(NULL);
				}
			}
		} 
		printf("\n");
	}
}
/*oi duo sunarthseis pou ulopoioun to pipe pipesource kai pipedestination*/
void pipesource(int fd[],char *pip1[]){
	int ppid;
	switch(ppid=fork()){
		case 0: /* child */
		dup2(fd[1], 1);
		close(fd[0]); 
		execvp(pip1[0], pip1);
		perror(pip1[0]);
	default: /* parent */
		wait(NULL); 
	case -1: 
		perror("fork");
		exit(1); 
	}
}

void pipedestination(int fd[],char *pip2[]){
	int ppid;
	switch (ppid=fork()){
		case 0: /* child */ 
			dup2(fd[0], 0);
			close(fd[1]);
			execvp(pip2[0], pip2);
			perror(pip2[0]);
		default: /* parent does nothing */
			wait(NULL);
		case -1:
			perror("fork"); 
			exit(1);
	}
}
