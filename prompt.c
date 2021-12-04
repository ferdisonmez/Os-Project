#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COMMAND_CH 80
#define MAX_COMMAND_WORD 10
#define R   "\x1B[31m"
#define G   "\x1B[32m"
#define B   "\x1B[34m"
#define RESET "\x1B[0m"

char** parseLine(int *numberOfWord, char *line, char *argv[])
{

   int i=0;
   *numberOfWord=0;
   const char space[2] = " ";
   char *word;
   

   if(line[0]=='\0'){
        printf("No parameter\n");
        return NULL;
   }
   else{
    
   word = strtok(line, space);//Split by space
   
 
   argv[i]=(char *)malloc((MAX_COMMAND_CH+1)*sizeof(char));
   strcpy(argv[i],word);

   i++;
   *numberOfWord=i;
   while(word!= NULL) {
   
      word = strtok(NULL, space);
    
      if(word==NULL){
        break;
      }

      argv[i]=(char *)malloc(40*sizeof(char));
      strcpy(argv[i],word);
           
      i++;
      *numberOfWord=i;
      if(*numberOfWord>10){*numberOfWord=0; printf("Many parameter\n");return NULL;}

   }  
   }   
  
   return argv;

}


int main(int argc)
{

   char cwd[50];
   char *buf=(char *)malloc(50*sizeof(char));
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       //printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }

    uid_t uid;
    char *username;
    uid = getuid();
    username = getlogin();
    
    cuserid(buf);
    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX + 1);
    char line[MAX_COMMAND_CH]; //Max numbers of character
    char *argv[64];  /* prompt argümanları/parametreleri     */
    char *argv2[64]; /* "shell" ile dosya okutulduğunda kullanılacak olan argüman dizisi*/

    char **argvTest;
    int wordNum=0;

    while(1){
        //signal(SIGINT, fonk);                      
     
    //printf(YEL "%s@%s:"RESET RED "%s > " RESET, getenv("LOGNAME"), hostn, getcwd(currentDirectory, 1024));
     
    printf(R "%s:" RESET G "%s >" RESET,cwd,buf);   
    
    if (fgets(line, sizeof(line), stdin)!=NULL) {    
        line[strcspn(line, "\n")] = '\0';        
    }   
                  
    argvTest=parseLine(&wordNum,line,argv);
    memset(line,'\0',MAX_COMMAND_CH*sizeof(char));
    for(int i=0; i<wordNum; i++)
    printf("%s\n",argvTest[i]);

    }

    if(argvTest!=NULL){
        free(argvTest);
    }
    
}