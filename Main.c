#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<sys/types.h>
#include <sys/wait.h>
#define MAX_COMMAND_CH 80
#define MAX_COMMAND_WORD 10
#define RED "\x1B[31m"
#define YEL "\x1B[33m"
#define RESET "\x1B[0m"
#define PIDCOUNT 5

static char* currentDirectory;
extern char** environ;


char * kWord[] = {
"exit",
"cd",
"showpid",
"pwd",
"ls",
"mkdir",
"cat",
"df",
"touch",
NULL

};

void SauShell(){
		printf("\n\t============================================\n");
		printf("\t Saü Shell \n");
		printf("\t--------------------------------------------\n");
		printf("\t \n");
		printf("\t============================================\n");
		printf("\n\n");
}

int controlCommand(char ** commandStr){

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
		return -1;
	}
	return -0;

}

int execute(char ** commandStr){
		int pid;
		pid = fork();

		if(pid < 0){
			printf("fork() olusturulamadi\n");
			exit(-1);
		}

		if(pid == 0){
			
			printf("%d",(int)getpid());
			if ( execvp(commandStr[0] , commandStr) == -1){
				printf("execvp hatası olustu\n");
				exit(0);
			}

		}
		else{



			waitpid(-1,NULL,0);
			return pid;
		}

}

char** parseLine(char *line, char *argv[])
{

int i=0;
int numberOfWord=0;
const char space[2] = " ";
char *word;

	if(line[0]=='\0'){
		printf("Parametre Yok\n");
		argv[0]='\0';
		return argv ;
	}
	else{

	word = strtok(line, space);
	argv[i]=(char *)malloc((MAX_COMMAND_CH+1)*sizeof(char));
	strcpy(argv[i],word);

	i++;
	numberOfWord=i;
		while(word!= NULL) {

			word = strtok(NULL, space);
				if(word==NULL){
					break;
				}

			argv[i]=(char *)malloc((MAX_COMMAND_CH+1)*sizeof(char));
			strcpy(argv[i],word);
			i++;
			numberOfWord=i;




					if( numberOfWord>MAX_COMMAND_WORD ){
						printf("Many parameter\n");
						return NULL;

					}
		}//While End

	}
	argv[i]=NULL;
	return argv;
}

void PromptYazdir()
{
	char* username =(char *)malloc(25*sizeof(char));
	memset(username, '\0', sizeof(username));
	cuserid(username);
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
	printf(YEL "%s" RESET RED " %s@%s: " RESET,getcwd(currentDirectory, 1024),username , hostn);
}

int main(int argc,char ** envp){
	environ=envp;
	SauShell();

	char line[MAX_COMMAND_CH+1]; //Max numbers of character
	char *argv[MAX_COMMAND_CH+1]; /* prompt argümanları/parametreleri */
	char** myArgV=(char**)malloc(sizeof(char*)*MAX_COMMAND_WORD);
	myArgV[0]='\0';
	int j ,i=0,k=0;
	int cPid=0;

	int childPid[PIDCOUNT];

	while(1){
	PromptYazdir();
	printf("sau > ");
	if (fgets(line, sizeof(line), stdin)!=NULL) {
		line[strcspn(line, "\n")] = '\0';
	}

	myArgV=parseLine(line,myArgV);
	memset(line,'\0',MAX_COMMAND_CH+1*sizeof(char));

	if(myArgV[0]==NULL){
		continue;
	}

	if ( controlCommand(myArgV) == -1){
		continue;
	}

	if( strcmp(myArgV[0],kWord[0] ) == 0){
			printf("exit\n" );
			printf("(kabuk sonlanır)\n" );
			exit(1);
	}
	else if( strcmp(myArgV[0],kWord[1] ) == 0 ){
		if( myArgV[1] != NULL ){
			if( chdir(myArgV[1]) < 0 ){
				printf("cd komut hata verdi.\n");
			}
		}
		continue;
	}
	else if( strcmp(myArgV[0],kWord[2] ) == 0 ){
		for( k = 0 ; k<i;k++){
			printf("%d\n",childPid[k]);
		}
		continue;
	}


	cPid = execute(myArgV);

	if (  cPid != -1){
		if(i>=PIDCOUNT){
			i=PIDCOUNT-1;
		}
		childPid[i] = cPid;
		i++;

	}

	for ( j = 0; myArgV[j]!=NULL; j++)//Free splitting command array
	{
		free(myArgV[j]);
	}
}//While

	free(myArgV);
	return 0;

}