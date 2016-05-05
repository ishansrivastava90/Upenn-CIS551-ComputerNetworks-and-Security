#include <stdio.h>
#include <stdlib.h>

char* execute_command(char *com)
{
	FILE *fd;
	char *buffer,*buf;
	fd=popen(com,"r");
	
        int size = 0;
	while (fgetc(fd) != EOF) {
	        size++;
    	}
	buf = malloc(size+1);
	pclose(fd);
 	fd=popen(com,"r");
	int i=0;
	int j=0;

	buffer=malloc(size+1);
	while(fgets(buf, size, fd))
	{
          i=0;
	  while(buf[i]!='\0')
          {
		buffer[j]=buf[i];
			j++;
			i++;
	  }
	}
	buffer[j]='\0';		
	if(buffer[0]=='\0')
	{
		buffer="No such file or directory";
	}
	
//	fprintf(stderr,"Variable : %s End\n",buffer);
	pclose(fd);
       
        return buffer;
}

/*int main()
{
    shell_commands("cat temp.txt");
}*/
