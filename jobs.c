#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
void jobstat(int shellid,char* home_path,char *jobstatus)
{
  char *procpath=(char *)malloc(30*sizeof(char));
  char *temp_path=(char *)malloc(100*sizeof(char));
  sprintf(procpath,"/proc/%d/stat",shellid);
  FILE *file;
  file=fopen(procpath,"r");
  if(!file)
  {
    perror("Error:");
    return;
  }
  else
  {
    char *cont=NULL;
    char tokens[50][30];
    // char *token=(char *)malloc(30*sizeof(char));
    size_t len=0;
    int args;
    if(getline(&cont,&len,file)==-1)
    {
      perror("Error");
      return;
    }//==-1 for error
    fclose(file);
    char *token=strtok(cont," ");

    for(args=0;token!=NULL;args++)
    {
      strcpy(tokens[args],token);
      token=strtok(NULL," ");
    }
    strcpy(jobstatus,tokens[2]);
  }
}
