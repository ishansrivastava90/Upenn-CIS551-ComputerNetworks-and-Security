#include <stdio.h>
#include <string.h>
void add_mac_user(char *);
void delete_mac_user(char *);

/*int main ()
{
   char command[100];
	char address[100];

   //strcpy( command, "iptables -A INPUT -m mac --mac-source cc:fa:00:cc:81:cd -j DROP" );
   //system(command);
	printf("Enter the Mac Address:\n");
	scanf("%s",address);
	delete_mac_user(address);
   return(0);
} 
*/
int add_mac(char * address, char* port)
{
	char command[100];
	char tail[50];
   	strcpy( command, "iptables -A INPUT -m mac --mac-source ");
	strcat(command,address);

	strcpy(tail," -p tcp --dport ");
	strcat(command,tail);
	strcat(command,port);

        strcpy(tail," -j ACCEPT");
	strcat(command,tail);
	
   	return system(command);
}

int delete_mac(char * address,char* port )
{
	char command[100];
	char tail[50];
   	strcpy( command, "iptables -D INPUT -m mac --mac-source ");
	strcat(command,address);
	
	strcpy(tail," -p tcp --dport ");
	strcat(command,tail);
	strcat(command,port);
	
	strcpy(tail," -j ACCEPT");
	strcat(command,tail);
	
   	return system(command);
}
