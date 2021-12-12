#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<sys/types.h>
#include <sys/wait.h>


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

static char* currentDirectory;
extern char** environ;



char * kWord[] = {
	"exit",
	"cd",
	"pwd",
	"showpid",
	"ls",
	"mkdir",
	"cat",
	"df",
	"touch",
	NULL
	
};



void welcomeScreen(){
        printf("\n\t============================================\n");
        printf("\t                   C Shell  \n");
        printf("\t--------------------------------------------\n");
        printf("\t                                  \n");
        printf("\t============================================\n");
        printf("\n\n");
}


void controlCommand(char ** commandStr){

	int flag=0;
	int i=0;
	while( kWord[i] != NULL ){

		if( strcmp(commandStr[0],kWord[i] ) == 0 ){
			flag =1;
			break;
		}
		i++;
	}

	if(flag == 0){
		printf("Hata : Komut icra edilemiyor\n" );
	}


}



void execute(char ** commandStr){
	int pid;
	pid = fork();

	if(pid < 0){
		printf("fork() return error \n");
		exit(-1);
	}


	if(pid == 0){
			
		controlCommand(commandStr);

		if ( execvp(commandStr[0] , commandStr) == -1){
			printf("execvp error is occured\n");
			exit(0);
			//kill(getpid() , SIGTERM );
		}
	}
	else{
			
		waitpid(-1,NULL,0);
			
	}


}


void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}


void PromptBas()
{
    char hostn[1204] = "";
    gethostname(hostn, sizeof(hostn));
    printf(YEL "%s@%s:"RESET RED "%s > " RESET, getenv("LOGNAME"), hostn, getcwd(currentDirectory, 1024));
}

int main(int argc,char ** argv,char ** envp){

	environ=envp;
	welcomeScreen();


	char * line = (char *)calloc(1024,sizeof(char));
	
	int * childReturn = (int*) calloc(1,sizeof(int));
	
	char * myArgV[80] /*= {"pwd",NULL}*/;
	while(1){

		PromptBas();
		printf("sau -> ");     /*   display a prompt             */
          gets(line);              /*   read in the command line     */
          parse(line, myArgV);       /*   parse the line               */
		

		if( strcmp(myArgV[0],kWord[0] ) == 0){
			printf("exit\n" );
			printf("(kabuk sonlanır)\n" );
			exit(1);
		}
		else if(  strcmp(myArgV[0],kWord[1] ) == 0 ){
			if( myArgV[1] != NULL ){
				if( chdir(myArgV[1]) < 0 ){
					printf("cd komut hata verdi.\n");
				}	
			}
			continue;
			
		}

		execute(myArgV);


	}

	return 0;


}