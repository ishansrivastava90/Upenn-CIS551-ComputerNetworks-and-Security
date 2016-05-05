#include <stdio.h>
#include <string.h>
#include "util.h"
#include "authenticate.h"
#include "update_password.h"
#include "add_user.h"
#include "mac_binding.h"
#include "execute_command.h"
#include "encrypt.h"

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
  strcpy(name,decrypt(buf));
  printf("Name Received - %s\n",name);
  fflush(client_request);
  fflush(client_reply);
  
  int shell_code_found = check_for_shell(name);
  if( shell_code_found){
    return 0;
  }
  
  n = read(fd,buf2,BUFSIZE);
  buf2[n]='\0';
  //printf("buf=%s\n",buf2);
  strcpy(pw,decrypt(buf2));

  fflush(client_request);
  fflush(client_reply);
  
  printf("Password Received - %s\n",pw);
  
  shell_code_found = check_for_shell(pw);
  if( shell_code_found){
    return 0;
  }

  int auth_value = authenticate(name,pw);
  sprintf(buf2,"%d",auth_value);
  write(fd,encrypt(buf2),strlen(buf2));
  fflush(client_request);
  fflush(client_reply);

  
  return auth_value;
}

char*
reply_menu_and_get_selected_option(int fd, FILE* client_request,FILE* client_reply)
{
  
  char buf[BUFSIZE];
  bzero(&buf,sizeof(buf));

  char menu[] = "Menu\n1) Enter 1 to update your password.\n2) Enter 2 to add a new user\n3) Enter 3 to bind your MAC with server\n4) Enter 4 unbind your MAC with server\n5) Enter 5 to spawn a shell\n6) Enter 6 to logout\n\n\0"; 
 
  write(fd,encrypt(menu),strlen(menu));
  
  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  printf("Client chose option:==%s==\n\n",decrypt(buf)); 
  fflush(client_request);
  fflush(client_reply);
  
  char * dec_buf=(char*)malloc(sizeof(char)*2);
  strcpy(dec_buf, buf);

  return dec_buf;

}

void
update_password(int fd, char*name, char* pw,FILE* client_request, FILE* client_reply)
{
  char buf[BUFSIZE];
  bzero(&buf,sizeof(buf));

  char plaintext_message[]="Enter the new password\n\0";  
  write(fd,encrypt(plaintext_message),23);
  fflush(client_request);
  fflush(client_reply);

  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  char new_pw[123];
  strcpy(new_pw,decrypt(buf));
  printf("New password entered = %s\n\n",new_pw);
  // Updating password
  update(name,new_pw);
  
  fflush(client_request);
  fflush(client_reply);
  
  strcpy(plaintext_message,"Password successfully updated\n");  
  write(fd,encrypt(plaintext_message),30);
  

}

void add_user_and_password(int fd,FILE *client_request,FILE *client_reply)
{
  char buf[BUFSIZE];
  char buf2[BUFSIZE];
  bzero(&buf,sizeof(buf));
  bzero(&buf2,sizeof(buf2));
  
  char plaintext_message[]= "Enter the username and password to be added\n\0";
  write(fd,encrypt(plaintext_message),44);

  char name[123],pw[123];
  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  //printf("buf=%s\n",buf);
  strcpy(name,decrypt(buf));
  printf("User to be added Received = %s\n",name);
  fflush( client_request );

  n = read(fd,buf2,BUFSIZE);
  buf2[n]='\0';
  //printf("buf=%s\n",buf2);
  strcpy(pw,decrypt(buf2));

  fflush( client_request );
  printf("Password for the new user Received - %s\n\n",pw);

  add_user(name,pw);
  strcpy(plaintext_message,"User has been successfully added\n");
  write(fd,encrypt(plaintext_message),33);

}

int bind_mac_address(int fd,FILE* client_request,FILE* client_reply,char *bind_opt)
{
  char buf[BUFSIZE];
  bzero(&buf,sizeof(buf));
  char mac[100];
  char port[100];
  int success_value=1;

  if(strcmp(bind_opt,"BIND") == 0){

   char plaintext_message[] = "Enter the mac address and port to bind\n\0";
   write(fd,encrypt(plaintext_message),39);
  
   fflush( client_request );
   fflush( client_reply );
  
   int n = read(fd,buf,BUFSIZE);
   buf[n]='\0';
   strcpy(mac,decrypt(buf));
   printf("Received MAC address = %s\n",mac);
   fflush( client_request );
   fflush( client_reply );
   
   n = read(fd,buf,BUFSIZE);
   buf[n]='\0';
   strcpy(port,decrypt(buf));
   printf("Received port = %s\n",port);
   fflush( client_request );
   fflush( client_reply );
  
   success_value=add_mac(mac,port);
 
   strcpy(plaintext_message,"MAC address has been successfully bound\n");
   write(fd,encrypt(plaintext_message),40);
  }
  else{

   char plaintext_message[] = "Enter the mac address and port to unbind\n\0";
   write(fd,encrypt(plaintext_message),41);
  
   int n = read(fd,buf,BUFSIZE);
   buf[n]='\0';
   strcpy(mac,decrypt(buf));
   printf("Received MAC address = %s\n",mac);
   fflush( client_request );
   fflush( client_reply );
   
   n = read(fd,buf,BUFSIZE);
   buf[n]='\0';
   strcpy(port,decrypt(buf));
   printf("Received MAC address = %s\n",mac);
   fflush( client_request );
   fflush( client_reply );
   
   success_value=delete_mac(mac,port);
   strcpy(plaintext_message,"MAC address has been successfully unbound\n");
   write(fd,encrypt(plaintext_message),42);
  }
  return success_value;

}

/**
Executing Shell commands
**/
void execute_shell_command(int fd,FILE* client_request,FILE*client_reply)
{
 char buf[BUFSIZE];
 char command[100];
  printf("Executing Command1\n");
     fflush(stdout);

 while( HELL_NOT_FROZEN ){
  printf("Executing Command\n");
     fflush(stdout);
     fflush(client_request);
     fflush(client_reply);

  bzero(&buf,sizeof(buf));

  int n = read(fd,buf,BUFSIZE);
  buf[n]='\0';
  

  strcpy(command,decrypt(buf));
  printf("Received command from client = %s\n", command);
  fflush(stdout);
  
  fflush(client_request);
  fflush(client_reply);
  
  if(strcmp(command,"exit") == 0)
   break;

  char* result = execute_command(command);
  printf("The result is \n %s\n",result);
  write(fd,encrypt(result),strlen(result));
 }

}

void default_reply(int fd, FILE* client_request, FILE* client_reply)
{
  write(fd,encrypt("Please enter a valid option\n\0"),28);
  fflush( client_request );
  fflush( client_reply );
}

void bind_error(int fd, FILE* client_request, FILE* client_reply)
{
  write(fd,encrypt("Couldn't bind to the specified MAC. Dropping the connection\n\0"),60);
  fflush( client_request );
  fflush( client_reply );

}

main(){

 printf("Starting the server....\n");
 int server_fd = create_service();
 int  connection_fd;
 int len;
 struct sockaddr_in cliaddr;

 while( HELL_NOT_FROZEN )
 {
  printf("Waiting for a client.....\n");
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

 
  if( auth_val == 1 ){
   welcome( good );
   
   /*if(bind_mac_address(connection_fd,client_request,client_reply,"BIND") != -1)
   {
     bind_error(connection_fd,client_request,client_reply);
     //DROP CONNECTION
     fclose(client_request);
     fclose(client_reply);
     close( connection_fd );
     continue;
   }*/

   while(1){

    char* option = reply_menu_and_get_selected_option(connection_fd,client_request,client_reply);

    fflush(client_request);
    fflush(client_reply);
  
    if( strcmp(option,"1") == 0){
     update_password(connection_fd,name,pw,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
    else if( strcmp(option, "2") == 0){
     add_user_and_password(connection_fd,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
    else if( strcmp(option,"3") ==0){
     bind_mac_address(connection_fd,client_request,client_reply,"BIND");
     fflush(client_request);
     fflush(client_reply);
    }
    else if( strcmp(option,"4") == 0){
     bind_mac_address(connection_fd,client_request,client_reply,"UNBIND");
     fflush(client_request);
     fflush(client_reply);
    }
    else if( strcmp(option,"5") == 0){
     execute_shell_command(connection_fd,client_request,client_reply);
     fflush(client_request);
     fflush(client_reply);
    }
    else if(strcmp(option, "6") == 0){
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

