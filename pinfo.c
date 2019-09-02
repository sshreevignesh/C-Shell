#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
void pinfo(int shellid,char* home_path)
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
    printf("pid -- %s\nProcess Status -- %s\nmemory -- %s\n",tokens[0],tokens[2],tokens[22]);
    char *exepath=(char *)malloc(30*sizeof(char));
    sprintf(exepath,"/proc/%d/exe",shellid);
    if(readlink(exepath,cont,200)==-1)
    {
      perror("Error");
      return;
    }
    cont[199]='\0';
    int homelen=strlen(home_path);
    strcpy(temp_path,cont);
    temp_path[homelen]='\0';
    if(homelen>strlen(cont)||strcmp(temp_path,home_path))
    {
      printf("Executable path -- %s\n",cont);
    }
    else
    {
      printf("Executable path -- ~%s\n",&cont[homelen]);
    }

  }
}
