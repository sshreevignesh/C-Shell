//Note: Since it is given that max no.of commands it should output is 10, we assume the <num><=10

#include<stdio.h>
#include<string.h>
#include<unistd.h>
void hist(char *c,int flag,char* hispath)
{
  int count=10;
  if(flag>1)
  {
    if(strcmp("10",c))
    {
      count=c[0]-'0';
    }
  }
  char *comm=(char *)malloc(100*sizeof(char));
  int fd1;
  fd1=open(hispath,O_CREAT|O_RDONLY, 777);
  int end = lseek(fd1,0,SEEK_END);
  count=count*(-100);
  int currpos=lseek(fd1,count,SEEK_END);
  if(currpos==-1)
  {
    currpos=lseek(fd1,0,SEEK_SET);
  }
  // printf("%d %d\n", currpos,end);
  while(currpos<end){
    read(fd1,comm,100);
    // comm[strlen(comm)-1]=='\0';
    printf("%s",comm);
    currpos+=100;
  }
  close(fd1);
}
