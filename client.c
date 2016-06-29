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
#define CHAVE_MEMORIA_COMPARTILHADA 10

int main(){


   char buffer[501];
   sem_t* ptr_mem;
   int len, mysocket, mem_id,key;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = htonl(INADDR_ANY); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(2000);                /* set destination port number */

/*
	Identifica e obtem acesso a area de memoria compartilhada	
*/

    mem_id = shmget(CHAVE_MEMORIA_COMPARTILHADA, sizeof(sem_t),0777|IPC_CREAT);
    if(mem_id < 0){
    	printf("Erro ao criar area de memoria compartilhada\n");
    	exit(0);
    }

    ptr_mem = (sem_t*)shmat(mem_id, (char*)0, 0);
    if(ptr_mem == NULL){
    	printf("Erro de mapeamento de memoria...\n");
    	exit(0);
    }	

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
/*   printf("CHEGOU AQUI\n");
   
   read(mysocket, key, sizeof(key));
   printf("LEITURA: %d\n",key);
*/
/*
	sem_wait, write(socket), sem_post
*/
	printf("Antes do sem_wait\n");
	sem_wait(ptr_mem);
	printf("Depois do sem_wait \n");
	sleep(20);	
	write(mysocket, "Ola sou o cliente 1\n", strlen("Ola\n")+1);	
	printf("Antes do sem_post \n");	
	sem_post(ptr_mem);	
	printf("Depois do sem_post \n");
   	//shmctl(mem_id, 0, IPC_RMID);
	close(mysocket);
 	return 0;
}
