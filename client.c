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
 
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
  
   len = recv(mysocket, buffer, 500, 0);

    buffer[len] = '\0';
 
   printf("Received %s (%d bytes).\n", buffer, len);
 
   close(mysocket);
 	return 0;
}
