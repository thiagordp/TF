#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){


   char buffer[501];
   int len, mysocket,key;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_ANY); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(2000);                /* set destination port number *
/*
	Faz a conexao com o servidor
 */

   if (connect(mysocket, (struct sockaddr*)&dest, sizeof(struct sockaddr)) < 0){
		printf("Erro\n");
		exit(0);
   }


/*
	Faz a leitura do que esta no socket
*/
   printf("CHEGOU AQUI\n");
   
/*   read(mysocket, key, sizeof(key));
   printf("LEITURA: %d\n",key);
*/
/*
	sem_wait, write(socket), sem_post
*/
	while(1);
	close(mysocket);
 	return 0;
}
