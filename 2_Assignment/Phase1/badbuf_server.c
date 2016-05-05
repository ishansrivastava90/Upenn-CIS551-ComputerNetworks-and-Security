#include <stdio.h>
#include <string.h>
#include "util.h"
#include "authenticate.h"
#include "update_password.h"
#include "add_user.h"
int match(char *s1, char *s2) {
    while( *s1 != '\0' && *s2 != '\0' && *s1 == *s2 ){
    s1++; s2++;

    }
return( *s1 - *s2 );
}

void welcome(char *str) { printf(str); }

void goodbye(char *str) { void exit(); printf(str); /*exit(1);*/ }

void service( int fd )
{

}

int create_service()
{
  int listenfd;
  struct sockaddr_in servaddr;

  listenfd = socket(AF_INET, SOCK_STREAM, 0 );
  if( listenfd < 0 )
    {
      perror( "creating socket for listenfd" );
      exit( ERR_SOCKET );
    }
  printf("Server Up - Socket Created....\n");

  bzero( &servaddr, sizeof(servaddr) );
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
  servaddr.sin_port = htons( CIS551_PORT );

  if( bind( listenfd, (SA *) &servaddr, sizeof(servaddr) ) < 0 )
  {
    perror( "bind on listenfd");
    exit( ERR_BIND );
  }
  printf("Server Up - Socket Bound...\n");

  if( listen( listenfd, LISTENQ ) < 0 )
    {
      perror( "listen on listenfd" );
      exit( ERR_LISTEN );
    }

  return( listenfd );
}

int
authenticate_request( int fd, FILE* client_request, FILE* client_reply, char *name,char *pw )
{

  char buf[BUFSIZE];
  char buf2[BUFSIZE];
  bzero(&buf,sizeof(buf));
  bzero(&buf2,sizeof(buf2));
  
  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  //printf("buf=%s\n",buf);
  strcpy(name,buf);
  printf("Name Received - %s\n",name);
  fflush(client_request);
  fflush(client_reply);
  
  n = read(fd,buf2,BUFSIZE);
  buf2[n]='\0';
  //printf("buf=%s\n",buf2);
  strcpy(pw,buf2);

  fflush(client_request);
  fflush(client_reply);
  
  printf("Password Received - %s\n",pw);

  int auth_value = authenticate(name,pw);
  sprintf(buf2,"%d",auth_value);
  write(fd,buf2,strlen(buf2));
  fflush(client_request);
  fflush(client_reply);

  
  return auth_value;
}

char
reply_menu_and_get_selected_option(int fd, FILE* client_request,FILE* client_reply)
{
  
  char buf[BUFSIZE];
  bzero(&buf,sizeof(buf));

  char menu[] = "Menu\n1) Enter 1 to update your password.\n2) Enter 2 to add a new user\n3) Enter 3 to logout\n\n\0"; 
 
  write(fd,menu,strlen(menu));
  
  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  printf("Client chose option: %s\n\n",buf); 
  fflush(client_request);
  fflush(client_reply);

  return buf[0];

}

void
update_password(int fd, char*name, char* pw,FILE* client_request, FILE* client_reply)
{
  char buf[BUFSIZE];
  bzero(&buf,sizeof(buf));
  
  write(fd,"Enter the new password\n\0",23);
  fflush(client_request);
  fflush(client_reply);

  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  char new_pw[123];
  strcpy(new_pw,buf);
  printf("New password entered = %s\n\n",new_pw);
  // Updating password
  update(name,new_pw);
  
  fflush(client_request);
  fflush(client_reply);
  
  write(fd,"Password successfully updated\n\0",30);
  

}

void add_user_and_password(int fd,FILE *client_request,FILE *client_reply)
{
  char buf[BUFSIZE];
  char buf2[BUFSIZE];
  bzero(&buf,sizeof(buf));
  bzero(&buf2,sizeof(buf2));
  
  write(fd,"Enter the username and password to be added\n\0",44);

  char name[123],pw[123];
  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  //printf("buf=%s\n",buf);
  strcpy(name,buf);
  printf("User to be added Received = %s\n",name);
  fflush( client_request );

  n = read(fd,buf2,BUFSIZE);
  buf2[n]='\0';
  //printf("buf=%s\n",buf2);
  strcpy(pw,buf2);

  fflush( client_request );
  printf("Password for the new user Received - %s\n\n",pw);

  add_user(name,pw);

  write(fd,"User has been successfully added\n\0",33);

}

void default_reply(int fd, FILE* client_request, FILE* client_reply)
{
  write(fd,"Please enter a valid option\n\0",28);
  fflush( client_request );
  fflush( client_reply );

}

main(){

 printf("Starting the server....\n");
 int server_fd = create_service();
 int  connection_fd;
 int len;
 struct sockaddr_in cliaddr;

 printf("Waiting for a client.....\n");
 while( HELL_NOT_FROZEN )
 {
  printf("..");
  len = sizeof( cliaddr );
  connection_fd = accept( server_fd, (SA *) &cliaddr, &len );

  
  if( connection_fd < 0 ){
   perror( "accept on server_fd" );
   exit( ERR_ACCEPT );
  }
  printf("Connection established\n\n");
  
  FILE *client_request, *client_reply, *fdopen();
  client_request = fdopen( connection_fd, "r" );
  if( client_request == (FILE *) NULL )
  {
    perror( "fdopen of client_request" );
    exit( 1 );
  }
  client_reply = fdopen( connection_fd, "w" );
  if( client_reply == (FILE *) NULL )
  {
    perror( "fdopen of client_reply" );
    exit( 1 );
  }
  
  char *good = "Welcome to The Machine!\n";
  char *evil = "Invalid identity, exiting!\n";
  char name[123], pw[123]; /* passwords are short! */

 int auth_val = authenticate_request(connection_fd,client_request,client_reply,name,pw);
 fflush(client_request);
 fflush(client_reply);

 
//printf("login: "); scanf("%s", name);
//printf("password: "); scanf("%s", pw);
  if( auth_val == 1 ){
   welcome( good );
   while(1){

    char option = reply_menu_and_get_selected_option(connection_fd,client_request,client_reply);
    fflush(client_request);
    fflush(client_reply);
  
    if( option == '1'){
     update_password(connection_fd,name,pw,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
    else if( option == '2'){
     add_user_and_password(connection_fd,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
    else if(option == '3'){
     break;
    }
    else{
     default_reply(connection_fd,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
   }
  }
  else
    goodbye(evil );
  
  fclose(client_request);
  fclose(client_reply);


  close( connection_fd );
 }
 

}

