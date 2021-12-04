#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<sys/types.h>
#include <sys/wait.h>


char * kWord[] = {
	"exit",
	"cd",
	"pwd",
	"showpid",
	"ls",
	NULL
	
};


int main(){

	
	int * childReturn = (int*) calloc(1,sizeof(int));
	int pid;

	while(1){

		char * myArgV[] = {"exit",	 NULL};


		pid = fork();

		if(pid < 0){
			printf("fork() return error \n");
			exit(-1);
		}


		if(pid == 0){
			
			int i=0;
			int flag=0;
			while( kWord[i] != NULL ){

				if( strcmp(myArgV[0],kWord[i] ) == 0 ){

					if( strcmp(myArgV[0],kWord[0] ) == 0){
						printf("exit\n" );
						exit(1);
						break;
					}
					flag =1;
					break;
				}
				i++;
			}

			

			if(flag == 0){
				printf("command not found\n" );
				return -1;
			}

			if ( execvp(myArgV[0] , myArgV) == -1){
				printf("execvp error is occured\n");
				exit(0);
				//kill(getpid() , SIGTERM );
			}
			
		}
		else{
			
			waitpid(-1,childReturn,0);
			printf("xxxxxxxxxxxx\n");
			printf("%d\n", *childReturn );
			if( *childReturn == 1){
				printf("(kabuk sonlanır)\n" );
			}
			
		}

		break;

	}





}