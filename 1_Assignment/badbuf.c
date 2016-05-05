#include <stdio.h>

int match(char *s1, char *s2) {
    while( *s1 != '\0' && *s2 != '\0' && *s1 == *s2 ){
    s1++; s2++;
  //  printf("&s1 = 0x%lx \n",&s1);
//    printf("&s2 = 0x%lx \n",&s2);

    }
return( *s1 - *s2 );
}

void welcome(char *str) { printf(str); }

void goodbye(char *str) { void exit(); printf(str); exit(1); }

main(){

//printf("main = 0x%lf \n", &main);
char name[12], pw[12]; /* passwords are short! */

//printf("&name[123] = 0x%lx \n",&name);
//printf("&pw[123] = 0x%lx \n",&pw);

char *good = "Welcome to The Machine!\n";
char *evil = "Invalid identity, exiting!\n";

//printf("good = 0x%lx , &good = 0x%lx \n",good, &good); 
//printf("evil = 0x%lx , &evil = 0x%lx \n",evil, &evil);

//printf("match = 0x%lx \n",&match); 
//printf("welcome = 0x%lx \n", &welcome); 
//printf("goodbye = 0x%lx \n", &goodbye); 
 
printf("login: "); scanf("%s", name);
printf("password: "); scanf("%s", pw);

if( match(name,pw) == 0 )
welcome( good );
else
goodbye(evil );
}

