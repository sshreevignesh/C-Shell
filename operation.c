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
#include "environ.c"

void operate(char *command,int home,int* procid,char* procname[100],int shellid,char *home_path)
{
  int fd1;
  int pipeargs;
  int start_flag=0,end_flag=0;
  char *hispath=(char *)malloc(300*sizeof(char));
  char *tempcmd=(char *)malloc(300*sizeof(char));
  char commands[10][300];
  sprintf(hispath,"%s/hist.txt",home_path);
  fd1=open(hispath,O_CREAT|O_RDWR|O_APPEND, 0777);
  char *fullcmd=(char *)malloc(300*sizeof(char));
  strcpy(fullcmd,command);
  start_flag=1;
  command =strtok(fullcmd,"|");
  for(pipeargs=0;command != NULL;pipeargs++)
  {
    strcpy(commands[pipeargs],command);
    command = strtok(NULL,"|");
  }
  // strcat(command," > temp.txt");
  // printf("%s\n",command);
  int p=0;
  while(p<pipeargs)
  {
    // command=strtok(NULL," ");
    char *command1=(char *)malloc(300*sizeof(char));
    strcpy(command1,commands[p]);
    if(p == pipeargs-1)
    {
      // printf("end\n");
      end_flag = 1;
    }

    // printf("%s\n",command1);
    int file_output = 0;
    int file_input = 0;
    int pipe_output=0;
    int pipe_input=0;
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
    //If there is no input redirection and it is not at the starting
    if(!file_input&&!start_flag)
    {
      freopen("input.txt", "r", stdin);
      file_input=1;
      pipe_input=1;
    }
    start_flag = 0;

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
      freopen(tokens[args-1], "w+", stdout);
      args=args-2;
    }
    if(!end_flag&&!file_output)
    {
      freopen("output.txt", "w+", stdout);
      file_output=1;
      pipe_output=1;
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
        write(fd1,command1,100);
        close(fd1);
        comm_cd(tokens[1]);
      }
    }

    else if(!strcmp(tokens[0],"pwd"))
    {
      write(fd1,command1,100);
      close(fd1);
      comm_pwd();
    }

    else if(!strcmp(tokens[0],"echo"))
    {
      write(fd1,command1,100);
      close(fd1);
      comm_echo(tokens,args);
    }

    else if(!strcmp(tokens[0],"exit"))
    {
      write(fd1,command1,100);
      close(fd1);
      comm_exit();
    }

    else if(!strcmp(tokens[0],"ls"))
    {
      write(fd1,command1,100);
      close(fd1);
      ls(tokens,args);
    }

    else if(!strcmp(tokens[0],"-history"))
    {
      write(fd1,command1,100);
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
        write(fd1,command1,100);
        close(fd1);
        pinfo(temp,home_path);
      }
      else
      {
        write(fd1,command1,100);
        close(fd1);
        pinfo(shellid,home_path);
      }
    }
    else if(!strcmp(tokens[0],"setenv"))
    {
      write(fd1,command1,100);
      close(fd1);
      setenvir(tokens[1],tokens[2]);
    }
    else if(!strcmp(tokens[0],"unsetenv"))
    {
      write(fd1,command1,100);
      close(fd1);
      unsetenvir(tokens[1]);
    }
    else
    {
      write(fd1,command1,100);
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
    if(pipe_input==1)
    {
      remove("input.txt");
      // printf("removed input\n" );
      pipe_input=0;
    }
    if(pipe_output==1)
    {
      rename("output.txt","input.txt");
      // printf("renamed output\n" );
      pipe_output=0;
    }
    p++;
  }
}
