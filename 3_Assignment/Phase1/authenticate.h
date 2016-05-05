#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct sym_list Head;
int authenticate(char * username, char * password)
{
 
 restore(DATABASE);
 char *pw = lookup(username);
 if( pw != NULL){
  int pwlen = strlen(pw);
  pw[pwlen-1]='\0';

  if(strcmp(pw,password) == 0)
   return 1;
  else
   return 0;
 }
 return 0;

}

