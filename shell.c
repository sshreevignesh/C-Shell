//Path length<100

#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include "display.c"
#include "operation.c"
int main(void)
{
  int shell_id=getpid();
  int *procid=(int *)malloc(100*sizeof(int));
  int *tempid=(int *)malloc(100*sizeof(int));
  char *procname[100];
  char *tempname[100];
  for(int i=0;i<100;i++)
  {
    procname[i]=(char *)malloc(100*sizeof(char));
    tempname[i]=(char *)malloc(100*sizeof(char));
  }
  char *path,*command,*temp_path,*home_path;
  int home;
  path=(char *)malloc(200 * sizeof(char));
  temp_path=(char *)malloc(200 * sizeof(char));
  home_path=(char *)malloc(200 * sizeof(char));
  command=(char *)malloc(100 * sizeof(char));
  //Check whether this returns null for error
  getcwd(path,200);
  home=strlen(path);
  strcpy(home_path,path);
  home_path[home]='\0';
  while(1)
  {
    int args;
    int status;
    char tokens[10][100];
    getcwd(path,200);
    if(path==NULL)
    {
      perror("Could not retrieve the current path");
      exit(0);
    }
    strcpy(temp_path,path);
    temp_path[home]='\0';
    if(home>strlen(path)||strcmp(temp_path,home_path))
    {
      disp(path,0);
    }
    else
    {
      disp(&path[home],1);
    }
    fgets(command,100,stdin);
    operate(command,home,procid,procname,shell_id,home_path);
    int j=0;
    for(int i=1;i<=procid[0];i++)
    {
      if(waitpid((pid_t)procid[i],&status,WNOHANG)!=0)
      {
        printf("Process %s with id %d has exited\n",procname[i],procid[i]);
      }
      else
      {
        j++;
        tempid[j]=procid[i];
        tempname[j]=procname[j];
      }
    }
    tempid[0]=j;
    for(int i=0;i<=j;i++)
    {
      procid[i]=tempid[i];
      procname[i]=tempname[i];
    }
  }
  return 0;
}
