//This will have a function that will take the command as input and call the corresponding functions to execute them
//Assumption: Function wont have more than 4 arguments
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include "builtin.c"
#include "ls.c"
#include "pinfo.c"
#include "bonus.c"

void operate(char *command,int home,int* procid,char* procname[100],int shellid,char *home_path)
{
  int fd1;
  char *hispath=(char *)malloc(300*sizeof(char));
  sprintf(hispath,"%s/hist.txt",home_path);
  fd1=open(hispath,O_CREAT|O_RDWR|O_APPEND, 0777);
  char *command1=(char *)malloc(300*sizeof(char));
  strcpy(command1,command);
  int file_output = 0;
  int file_input = 0;
  int args;
  char tokens[10][100];
  char *token=strtok(command1," ");
  for(args=0;token!=NULL;args++)
  {
    strcpy(tokens[args],token);
    token=strtok(NULL," ");
  }

  //Removing the \n at the end of the command
  if(tokens[args-1][strlen(tokens[args-1])-1]=='\n')
  {
    tokens[args-1][strlen(tokens[args-1])-1]='\0';
  }


  //Changing input if there is input redirection
  for(int i=0;i<args;i++)
  {
    if(tokens[i][0]=='<')
    {
      freopen(tokens[i+1], "r", stdin);
      file_input=1;
      while(i<args)
      {
        strcpy(tokens[i],tokens[i+2]);
        i++;
      }
      args=args-2;
    }
  }


  //Changing output to file if there is redirection
  if(tokens[args-2][0]=='>'&&tokens[args-2][1]=='>')
  {
    file_output=1;
    freopen(tokens[args-1], "a+", stdout);
    args=args-2;
  }
  else if(tokens[args-2][0]=='>')
  {
    file_output=1;
    freopen(tokens[args-1], "w+", stdout) ;
    args=args-2;
  }


  //Checking The Command
  if(!strcmp(tokens[0],"cd"))
  {
    if(args<2)
    {
      printf("Error: No path provided for command cd\n");
    }
    else
    {
      write(fd1,command,100);
      close(fd1);
      comm_cd(tokens[1]);
    }
  }

  else if(!strcmp(tokens[0],"pwd"))
  {
    write(fd1,command,100);
    close(fd1);
    comm_pwd();
  }

  else if(!strcmp(tokens[0],"echo"))
  {
    write(fd1,command,100);
    close(fd1);
    comm_echo(tokens,args);
  }

  else if(!strcmp(tokens[0],"exit"))
  {
    write(fd1,command,100);
    close(fd1);
    comm_exit();
  }

  else if(!strcmp(tokens[0],"ls"))
  {
    write(fd1,command,100);
    close(fd1);
    ls(tokens,args);
  }

  else if(!strcmp(tokens[0],"-history"))
  {
    write(fd1,command,100);
    close(fd1);
    hist(tokens[1],args,hispath);
  }

  else if(!strcmp(tokens[0],"pinfo"))
  {
    if(args>1)
    {
      int temp=0;;
      for(int j=0;j<strlen(tokens[1]);j++)
      {
        if(!isdigit(tokens[1][j]))
        {
          printf("Error: The given pid is not valid\n");
          return;
        }
        temp*=10;
        temp+=tokens[1][j]-'0';
      }
      write(fd1,command,100);
      close(fd1);
      pinfo(temp,home_path);
    }
    else
    {
      write(fd1,command,100);
      close(fd1);
      pinfo(shellid,home_path);
    }
  }

  else
  {
    write(fd1,command,100);
    close(fd1);
    char *toks[10];
    for(int i=0;i<10;i++)
    {
      toks[i]=(char *)malloc(100*sizeof(char));
    }
    for(int i=0;i<args;i++)
    {
      strcpy(toks[i],tokens[i]);
    }
    toks[args]=NULL;

    pid_t child;
    int status;
    child=fork();
    if(child==0)
    {
      execvp(tokens[0],toks);
      //Give error here, since it will return from execvp only if it fails
      perror("Error");
    }
    else
    {
      pid_t dead;
      if(strcmp("&",tokens[args-1]))
      {
        do{
          dead=waitpid(child,&status,0);
        }while(child!=dead);
      }
      else
      {
        procid[++procid[0]]=(int) child;
        strcpy(procname[procid[0]],tokens[0]);
      }
    }
  }

  //Changing Output back to stdout
  if(file_output)
  {
    file_output=0;
    freopen("/dev/tty", "w", stdout);
  }

  //Changing Input back to stdin
  if(file_input)
  {
    // printf("kAshTapAdRa f0RmeRs sOn\n");
    file_input=0;
    freopen("/dev/tty","r",stdin);
  }
}
