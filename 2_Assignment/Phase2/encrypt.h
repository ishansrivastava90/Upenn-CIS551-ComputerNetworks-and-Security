#include<stdio.h>
#include<stdlib.h>
char * encrypt(char *);
char * decrypt(char *);

/*int main()
{
	int i=0;
	int d;
	d=0;
	char input[123];
	char* encrypted, * decrypted;
	printf("Enter the input string: \n");
	scanf("%s",input);
	encrypted=encrypt(input);
	printf("Encrypted : %s\n",encrypted);
	decrypted=decrypt(encrypted);
	printf("Decrypted : %s\n",decrypted);
	fflush(stdout);
	return 0;
}*/

char * encrypt(char * input)
{
	int i=0;
	int key=25;
	while (input[i]!='\0')
	{
		input[i]=(char)(input[i]+key);
		i++;
		if(key==0)
			key=25;
		key--;
	}
	input[i]='\0';
	return input;
}

char * decrypt(char * input)
{
	int i=0;
	int key=25;
	while (input[i]!='\0')
	{
		input[i]=(char)(input[i]-key);
		i++;
		if(key==0)
			key=25;
		key--;
	}
	input[i]='\0';
	return input;
}

int check_for_shell(char * input)
{
	char check[]="bin/sh";
	int result=0;
	if( strstr(input,check)!=NULL)
	{
		result=1;
	}
	return result;

}
