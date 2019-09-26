#include <stdlib.h>
#include <stdio.h>
#include<errno.h>
void comm_cd(char *command)
{
  // printf("%s\n",command );
  if(chdir(command)==-1)
  {
    perror("Error");
    return;
  }
}
void comm_pwd()
{
  char *path=(char *)malloc(200*sizeof(char));
  getcwd(path,200);
  if(path==NULL)
  {
    perror("Error");
    return;
  }
  printf("%s\n",path);
  free(path);
}
void comm_echo(char tokens[10][100],int args)
{
  for(int i=1;i<args;i++)
  {
    printf("%s ",tokens[i]);
  }
  printf("\n");
}
void comm_exit(int pid)
{
  kill(0,SIGKILL);
}
