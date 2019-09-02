#include<sys/utsname.h>
#include<stdio.h>
void disp(char *path,int flag)
{
  struct utsname names;
  uname(&names);
  if(flag)
  {
    printf("%s@%s:%c%s>",names.nodename,names.sysname,'~',path);
  }
  else
  {
    printf("%s@%s:%s>",names.nodename,names.sysname,path);
  }
}
