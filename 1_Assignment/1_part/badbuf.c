#include <stdio.h>

int match(char *s1, char *s2) {
    while( *s1 != '\0' && *s2 != '\0' && *s1 == *s2 ){
    s1++; s2++;

    }
return( *s1 - *s2 );
}

void welcome(char *str) { printf(str); }

void goodbye(char *str) { void exit(); printf(str); exit(1); }

main(){

char name[123], pw[123]; /* passwords are short! */

char *good = "Welcome to The Machine!\n";
char *evil = "Invalid identity, exiting!\n";
 
printf("login: "); scanf("%s", name);
printf("password: "); scanf("%s", pw);

if( match(name,pw) == 0 )
welcome( good );
else
goodbye(evil );
}

