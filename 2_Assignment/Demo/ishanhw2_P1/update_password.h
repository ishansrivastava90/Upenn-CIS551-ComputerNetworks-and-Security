#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct sym_list Head;
int update(char * username, char * password)
{
        printf("\nUpdating Password for user = %s...\n",username);

        restore(DATABASE);

        int pwlen = strlen(password);
        char new_pw[pwlen+1];

        strcpy(new_pw,password);
        new_pw[pwlen]='\n';
        new_pw[pwlen+1]='\0';
        
        insert(username,new_pw); 
        save(DATABASE);
        
}

