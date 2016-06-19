#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(){


   char buffer[501];
   int len, mysocket;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_ANY); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(2000);                /* set destination port number */
	if (connect(mysocket, (struct sockaddr*)&dest, sizeof(struct sockaddr)) < 0){
		printf("Erro\n");
		exit(0);
   }
  
   read(mysocket, buffer, sizeof(buffer));
sleep(3);	
     
   printf("Received \n%s",buffer);
 
   close(mysocket);
 	return 0;
}
