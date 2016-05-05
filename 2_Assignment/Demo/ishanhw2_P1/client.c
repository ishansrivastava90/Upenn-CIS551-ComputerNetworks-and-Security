#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include<string.h>

main(int argc, char *argv[])
{
	int sockfd = 0,n;
	char recvBuff[1024];
	char str[2],var[2],choice[2];
	char opt_1[2],opt_2[2],opt_3[2],result[100],message[100];
	char new_name[123],new_pwd[123];
	char name[123], pw[123]; /* passwords are short! */
	char *good = "Welcome to  The Machine!\n";
	char *evil = "Invalid identity, exiting!\n";
	
	struct sockaddr_in serv_addr; 
	FILE *server_request,*server_reply;

    	if(argc != 2)
    	{
        	printf("\n Usage: %s <ip of server> \n",argv[0]);
        	return 1;
    	} 
    	memset(recvBuff, '0',sizeof(recvBuff));
    	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("\n Error : Could not create socket \n");
        	return 1;
    	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(10551); 

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    	{
	        printf("\n inet_pton error occured\n");
	        return 1;
    	} 

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
       		printf("\n Error : Connect Failed \n");
       		return 1;
    	} 
	printf("Connected To The Server\n");

	server_request = fdopen( sockfd, "w" );
	if( server_request == (FILE *) NULL )
	{
	      perror( "fdopen of stream for server requests" );
	      exit( 2 );
	}

	server_reply = fdopen( sockfd, "r" );

  	if( server_reply == (FILE *) NULL )
    	{
      		perror( "fdopen of stream for server replies" );
     		 exit( 3 );
    	}

	
	printf("login: "); scanf("%s", name);
	write(sockfd,name,strlen(name));
        fflush( server_request ); 

	printf("password: "); scanf("%s", pw);
	write(sockfd,pw,strlen(pw));
        fflush( server_request ); 
		
	n=read(sockfd,str,sizeof(str));	
	fflush( server_reply );
	fflush( server_request );
	strcpy(var,"1\0");
	strcpy(opt_1,"1\0");
	strcpy(opt_2,"2\0");
	strcpy(opt_3,"3\0");

	if(str[0]==var[0])
	{
		printf("%s",good);
		while(1)
		{
		n=read(sockfd,result,sizeof(result));
		printf("%s",result);	
		fflush( server_reply );
		
		scanf("%s",choice);
		write(sockfd,choice,strlen(choice));		
		fflush( server_request );
		fflush( server_reply );

		if(choice[0]==opt_1[0])
		{
			n=read(sockfd,message,sizeof(message));
			message[n]='\0';	
			printf("%s",message);
			fflush( server_reply );
	
			scanf("%s",new_pwd);			
			write(sockfd,new_pwd,strlen(new_pwd));
			fflush( server_request );
			fflush( server_reply );

			n=read(sockfd,result,sizeof(result));
			result[n]='\0';			
			printf("%s",result);
			fflush( server_reply );
		}
		else if(choice[0]==opt_2[0])
		{
			n=read(sockfd,message,sizeof(message));
			message[n]='\0';
			printf("%s",message);
			fflush( server_reply );
			
			scanf("%s",new_name);
			write(sockfd,new_name,strlen(new_name));
			fflush( server_request );

			scanf("%s",new_pwd);
			write(sockfd,new_pwd,strlen(new_pwd));
			fflush( server_request );

			n=read(sockfd,result,sizeof(result));
			result[n]='\0';
			printf("%s",result);
			fflush( server_reply );
		}
		else if(choice[0]==opt_3[0])
		{
			fclose( server_request );
  			fclose( server_reply );
 			close( sockfd);
			exit(0);
		}
		else 
		{
			n=read(sockfd,message,sizeof(message));
			message[n]='\0';
			printf("%s",message);
			fflush( server_reply );
		}
		
		
	}
	}
	else
	{
		printf("%s",evil);
	}

	fclose( server_request );
  	fclose( server_reply );
 	close( sockfd);
}
