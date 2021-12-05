#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h>
#include<sys/types.h>
#include <sys/wait.h>
#define MAX_COMMAND_CH 80
#define MAX_COMMAND_WORD 10

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



void SauShell(){
        printf("\n\t============================================\n");
        printf("\t                   Saü Shell  \n");
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
		printf("fork() olusturulamadi\n");
		exit(-1);
	}


	if(pid == 0){
			
		controlCommand(commandStr);

		if ( execvp(commandStr[0] , commandStr) == -1){
			printf("execvp hatası olustu\n");
			exit(0);
		}
	}
	else{
			
		waitpid(-1,NULL,0);	
	}


}

char** parseLine(int *numberOfWord, char *line, char *argv[])
{

   int i=0;
   *numberOfWord=0;
   const char space[2] = " ";
   char *word;
   

   if(line[0]=='\0'){
        printf("Parametre Yok\n");
        return NULL;
   }
   else{
    
   word = strtok(line, space);
   
 
   argv[i]=(char *)malloc((MAX_COMMAND_CH+1)*sizeof(char));
   strcpy(argv[i],word);

   i++;
   *numberOfWord=i;
   while(word!= NULL) {
   
      word = strtok(NULL, space);
    
      if(word==NULL){
        break;
      }

      argv[i]=(char *)malloc((MAX_COMMAND_CH+1)*sizeof(char));
      strcpy(argv[i],word);
           
      i++;
      *numberOfWord=i;
      if(*numberOfWord>10){*numberOfWord=0; printf("Many parameter\n");return NULL;}

   }  
   }   
    
   return argv;

}

void PromptYazdir()
{
    char* username = getenv("USER");
    char hostn[1204] = "";
    gethostname(hostn, sizeof(hostn));
    printf(RED "%s@%s:"RESET BLU "%s > " RESET,username , hostn, getcwd(currentDirectory, 1024));
}

int main(int argc,char ** envp){
	environ=envp;
	SauShell();

    char line[MAX_COMMAND_CH+1]; //Max numbers of character
    char *argv[MAX_COMMAND_CH+1];  /* prompt argümanları/parametreleri     */
	char** myArgV=(char**)malloc(sizeof(char*)*1);
    myArgV[0]="\0";
    int wordNum=0;
    int j=0;

	while(1){

		PromptYazdir();
		printf("Sau :");  

    if (fgets(line, sizeof(line), stdin)!=NULL) {    
        line[strcspn(line, "\n")] = '\0';        
    }   
                  
    myArgV=parseLine(&wordNum,line,argv);
    memset(line,'\0',MAX_COMMAND_CH*sizeof(char));    
		
		if( strcmp(myArgV[0],kWord[0] ) == 0){
			printf("exit\n" );
			printf("(Kabuk Sonlandı!!!)\n" );
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

          for ( j = 0;j<wordNum; j++)
          {
             free(myArgV[j]);
          }
        
	}//While End
    free(myArgV);
	return 0;


}
