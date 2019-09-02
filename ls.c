#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

void sfile(char const filename[]){

  struct stat sfile;

  if(stat(filename,&sfile)==-1){
    perror("Error");
  }

}

void ls(char tokens[10][100],int args)
{
  int a=0,l=0;
  char *path =(char*)malloc(100*sizeof(char));
  strcpy(path,".");

  if(args>1&&tokens[args-1][0]!='-')
  {
    strcpy(path,tokens[args-1]);
  }

  else if(args>1&&tokens[1][0]=='-')
  {
    if(tokens[1][1]=='a')
    {
      a=1;
      if(tokens[1][2]=='l')
      {
        l=1;
      }
    }
    if(tokens[1][1]=='l')
    {
      l=1;
      if(tokens[1][2]=='a')
      {
        a=1;
      }
    }
  }



  char *file = (char*)malloc(100*sizeof(char));
  char perm[15]="----------";
  struct dirent *dir;
  DIR *curr = opendir(path);
  if(curr==NULL)
  {
    perror("Error:Cant open the directory");
  }
  dir=readdir(curr);
  while(dir!=NULL)
  {
    file=dir->d_name;
    if(file[0]=='.'&&a==0)
    {
      dir=readdir(curr);
      continue;
    }
    struct stat sfile;
    int temp=stat(file,&sfile);
    // if(temp==-1){
    //   perror("Error");
    // }
    if(l==1)
    {
      if(S_ISDIR(sfile.st_mode))
      perm[0] = 'd';
      if(sfile.st_mode & S_IRUSR)
      perm[1] = 'r';
      if(sfile.st_mode & S_IWUSR)
      perm[2] = 'w';
      if(sfile.st_mode & S_IXUSR)
      perm[3] = 'x';
      if(sfile.st_mode & S_IRGRP)
      perm[4] = 'r';
      if(sfile.st_mode & S_IWGRP)
      perm[5] = 'w';
      if(sfile.st_mode & S_IXGRP)
      perm[6] = 'x';
      if(sfile.st_mode & S_IROTH)
      perm[7] = 'r';
      if(sfile.st_mode & S_IWOTH)
      perm[8] = 'w';
      if(sfile.st_mode & S_IXOTH)
      perm[9] = 'x';

      printf("%s %ld ",perm,sfile.st_nlink);

      //user
      struct passwd *user;
      user = getpwuid(sfile.st_uid);
      printf("%s ", user->pw_name);

      //Group
      struct group *grp;
      grp = getgrgid(sfile.st_gid);
      printf("%s ",grp->gr_name);

      char* timestr=(char *)malloc(100*sizeof(char));
      timestr=ctime(&sfile.st_mtime);
      timestr[strlen(timestr)-1]='\0';
      //Assumption: the number of digits of memory is <=8
      printf("%8zu %s %s\n",sfile.st_size,timestr,file );
    }
    else
    {
        printf("%s\n",file);
    }
    dir=readdir(curr);
  }
}
