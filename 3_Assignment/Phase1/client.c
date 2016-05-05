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

main(int argc, char *argv[])
{
	int sockfd = 0,n;
	char recvBuff[1024];
	char str[2],var[2],choice[2];
	char opt_1[2],opt_2[2],opt_3[2],opt_4[2],opt_5[2],opt_6[2],result[201],message[500];
	char new_name[123],new_pwd[123],new_mac[20],new_port[20],command[20],com[20];
	char name[123], pw[123]; /* passwords are short! */
	char *temp;
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

	char *temp_name,*temp_pw,*temp_str,*temp_result,*temp_message,*temp_choice,*temp_new_pwd,*temp_new_name,*temp_new_mac,*temp_new_port;
	char *temp_command;	
	printf("login: "); scanf("%s", name);
	temp_name=encrypt(name);
	strcpy(name,temp_name);
	write(sockfd,name,strlen(name));
        fflush( server_request ); 

	printf("password: "); scanf("%s", pw);
	temp_pw=encrypt(pw);
	strcpy(pw,temp_pw);
	write(sockfd,pw,strlen(pw));
        fflush( server_request ); 
		
	n=read(sockfd,str,sizeof(str));
	temp_str=decrypt(str);
	strcpy(str,temp_str);	
	fflush( server_reply );
	fflush( server_request );
	strcpy(var,"1\0");
	int a=str[0];
	int b=var[0];
	strcpy(opt_1,"1\0");
	strcpy(opt_2,"2\0");
	strcpy(opt_3,"3\0");
	strcpy(opt_4,"4\0");
	strcpy(opt_5,"5\0");
	strcpy(opt_6,"6\0");
	if(a==b)
	{
		printf("%s",good);
		/*n=read(sockfd,message,sizeof(message));
		temp_message=decrypt(message);
		strcpy(message,temp_message);
		message[n]='\0';	
		printf("%s",message);
		fflush( server_reply );
	
		scanf("%s",new_pwd);	
		temp_new_pwd=encrypt(new_pwd);
		strcpy(new_pwd,temp_new_pwd);		
		write(sockfd,new_pwd,strlen(new_pwd));
		fflush( server_request );
		fflush( server_reply );

		n=read(sockfd,result,sizeof(result));
		temp_result=decrypt(result);
		strcpy(result,temp_result);
		result[n]='\0';			
		printf("%s",result);
		fflush( server_reply );*/

		while(1)
		{
		n=read(sockfd,result,sizeof(result));
		temp_result=decrypt(result);
		//strcpy(result,temp_result);
		printf("%s",temp_result);	
		fflush( server_reply );
		
		scanf("%s",choice);
		char ch[2];
		strcpy(ch,choice);

		temp_choice=encrypt(choice);
		//strcpy(choice,temp_choice);
		write(sockfd,temp_choice,strlen(temp_choice));	
		
		fflush( server_request );
		fflush( server_reply );
		
		if(ch[0]==opt_1[0]) //change pwd
		{
			n=read(sockfd,message,sizeof(message));
			temp_message=decrypt(message);
			//strcpy(message,temp_message);
			message[n]='\0';	
			fflush( server_reply );
		
			scanf("%s",new_pwd);	
			temp_new_pwd=encrypt(new_pwd);
			strcpy(new_pwd,temp_new_pwd);		
			write(sockfd,new_pwd,strlen(new_pwd));
			fflush( server_request );
			fflush( server_reply );

			n=read(sockfd,result,sizeof(result));
			temp_result=decrypt(result);
			strcpy(result,temp_result);
			result[n]='\0';			
			printf("%s",result);
			fflush( server_reply );
		}
		else if(ch[0]==opt_2[0]) //add new user
		{
			
			n=read(sockfd,message,sizeof(message));
			temp_message=decrypt(message);
			strcpy(message,temp_message);
			message[n]='\0';
			printf("%s",message);
			fflush( server_reply );
			
			scanf("%s",new_name);
			temp_new_name=encrypt(new_name);
			strcpy(new_name,temp_new_name);
			write(sockfd,new_name,strlen(new_name));
			fflush( server_request );

			scanf("%s",new_pwd);
			temp_new_pwd=encrypt(new_pwd);
			strcpy(new_pwd,temp_new_pwd);
			write(sockfd,new_pwd,strlen(new_pwd));
			fflush( server_request );

			n=read(sockfd,result,sizeof(result));
			temp_result=decrypt(result);
			strcpy(result,temp_result);
			result[n]='\0';
			printf("%s",result);
			fflush( server_reply );
		}		
		else if(ch[0]==opt_3[0]) //bind
		{
			n=read(sockfd,message,sizeof(message));
			temp_message=decrypt(message);
			strcpy(message,temp_message);
			message[n]='\0';	
			printf("%s",message);
			fflush( server_reply );
	
			scanf("%s",new_mac);	
			temp_new_mac=encrypt(new_mac);
			strcpy(new_mac,temp_new_mac);		
			write(sockfd,new_mac,strlen(new_mac));
			fflush( server_request );
			fflush( server_reply );
		
			scanf("%s",new_port);	
			temp_new_port=encrypt(new_port);
			strcpy(new_port,temp_new_port);		
			write(sockfd,new_port,strlen(new_port));
			fflush( server_request );
			fflush( server_reply );

			n=read(sockfd,result,sizeof(result));
			temp_result=decrypt(result);
			strcpy(result,temp_result);
			result[n]='\0';			
			printf("%s",result);
			fflush( server_reply );	
		}
		else if(ch[0]==opt_4[0]) //unbind
		{
			n=read(sockfd,message,sizeof(message));
			temp_message=decrypt(message);
			strcpy(message,temp_message);
			message[n]='\0';	
			printf("%s",message);
			fflush( server_reply );
	
			scanf("%s",new_mac);	
			temp_new_mac=encrypt(new_mac);
			strcpy(new_mac,temp_new_mac);		
			write(sockfd,new_mac,strlen(new_mac));
			fflush( server_request );
			fflush( server_reply );

			scanf("%s",new_port);	
			temp_new_port=encrypt(new_port);
			strcpy(new_port,temp_new_port);		
			write(sockfd,new_port,strlen(new_port));
			fflush( server_request );
			fflush( server_reply );

			n=read(sockfd,result,sizeof(result));
			temp_result=decrypt(result);
			strcpy(result,temp_result);
			result[n]='\0';			
			printf("%s",result);
			fflush( server_reply );
		}
		else if(ch[0]==opt_5[0]) //shell
		{
			printf("Use exit to exit the shell and return to the main menu\n");
			int i=0,j=0;
			char filename[10][100];
			fflush(stdout);
			while(1)
			{
				scanf("%s",command);
				if(strcmp(command,"cat")==0)
				{
					scanf("%s",filename[i]);
					/*while(strcmp(filename[i],"\n")!=0)	
					{
						i++;
						scanf("%s",filename[i]);
					}*/
					for(j=0;j<=i;++j)
					{
						strcat(command," ");
						strcat(command,filename[j]);
						strcat(command," ");
					}
				}
				strcpy(com,command);
				temp_command=encrypt(command);
				write(sockfd,temp_command,strlen(temp_command));
				
				if(strcmp(com,"exit")==0)
				break;				

				fflush( server_request );
				fflush( server_reply );

				n=read(sockfd,message,sizeof(message));
				temp_message=decrypt(message);
				strcpy(message,temp_message);
				message[n]='\0';	
				printf("%s",message);
				fflush( server_reply );
			}
		}
		else if(ch[0]==opt_6[0]) //logout
		{
			fclose( server_request );
  			fclose( server_reply );
 			close( sockfd);
			exit(0);
		}
		else 
		{
			n=read(sockfd,message,sizeof(message));
			temp_message=decrypt(message);
			strcpy(message,temp_message);
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
