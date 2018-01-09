#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


int main()
{
  signal(SIGINT, SIG_IGN); //ignoring ctrl+c
  char buf[50];
  int index;
  char *p;
  char *array[10];
  pid_t child_pid;
  int stat_loc;
  time_t rawtime;   //make hell prompt the current time
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  while(1){
    char *realtime = asctime (timeinfo);
    realtime[strlen(realtime)-1] = '#';
    printf("%s",realtime);
    fgets(buf,200,stdin);
    index=0;
    if(strlen(buf)>1){
      buf[strlen(buf)-1] = '\0'; //remove trailing newline
      p = strtok(buf," ");

      while(p!=NULL){
        array[index] = p;
        index++;
        p = strtok(NULL," ");
      }

      if(index>0 && strcmp(array[index-1],"&")==0){ //not allowing the shell to handle background processes by setting "&" to NULL if there is any
        array[index-1] = NULL;
        }else{
        array[index] = NULL;
      }
      if(strcmp(array[0],"exit")==0){    //“exit” to terminate the shell

      printf("%s\n","exit the shell" );
      break;
      }

      child_pid = fork(); //create child process
      if(child_pid<0){
       printf("%s\n","fork failed");
        exit(1); }
      if (child_pid == 0) {
       if(execvp(array[0], array)<0){  //use child process to run a command
         printf("%s\n","execvp failed");
         exit(1);}}
      else {
       if(waitpid(child_pid, &stat_loc, WUNTRACED)<0){ //parent process waiting for child process to finish
        printf("%s\n","waitpid failed");
        exit(1);}}
      }
    }
    
    return 0;

  }
