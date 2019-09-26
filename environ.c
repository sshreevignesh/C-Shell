#include<string.h>
void setenvir(char* var, char *value)
{
  if(var==NULL)
  {
    printf("Error: Too less arguments\n");
  }
  else if(value==NULL)
  {
    setenv(var," ",1);
  }
  else
  {
    setenv(var,value,1);
  }
}
void unsetenvir(char *var)
{
  if(var==NULL)
  {
    printf("Error: No arguments\n");
  }
  else
  {
    unsetenv(var);
  }

}
