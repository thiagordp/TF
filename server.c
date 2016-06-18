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

/*
    include das structs no programa cliente, para o mesmo poder ler na memória compartilhada a estrutura de arquivos
*/


void *Options(void *c);

sem_t s_servidor, s_cliente;



int main()
{
    int socket_des, fromlen, sock_des_cli, tamanho_cliente, proc_filho, mem_id, ptr_mem; //
    struct sockaddr_in servidor, sock_cli;
    char msg_buffer[80];
    int ultimoUser = 0;

/* 

	 socket() - cria o socket
     AF_INET - domínio do socket (internet).
     SOCK_STREAM - tipo de socket
     0 - protocolo de com. (IP)
     
*/
    if ((socket_des = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Erro ao criar o socket do servidor\n");
        exit(0);
    }

    // Atribuição das características do socket.
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(2000);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(servidor.sin_zero), 8);

    // Relaciona o descritor a referência da struct.
    if (bind(socket_des, (struct sockaddr *) &servidor, sizeof(servidor)) < 0)
    {
        fprintf(stderr, "Erro de bind\n");
        close(socket_des);  // Fecha
        exit(0);
    }

/*

	Criacao da area de memoria compartilhada
	shmget = criação de uma área de memória compartilhada
	shmat = mapeamento de uma área de memória compartilhada
	
*/    

    mem_id = shmget(CHAVE_MEMORIA_COMPARTILHADA, sizeof(int)*256,0777|IPC_CREAT);
    if(mem_id < 0){
    	printf("Erro ao criar area de memoria compartilhada\n");
    	exit(0);
    }

    ptr_mem = (int*)shmat(mem_id, (char*)0, 0);
    if(ptr_mem == NULL){
    	printf("Erro de mapeamento de memoria...\n");
    	exit(0);
    }

/* 
	
		Habilita o serv. para receber até 10 conexões.
     	socket_des - socket
     	10 - nº máximo de conexões.

*/
    if (listen(socket_des, 10) < 0){
        fprintf(stderr, "Erro de listen\n");
        exit(0);
    }else{
        printf("Servidor pronto para conexoes....\n\n");
        sem_init(&s_servidor, 0, 1);
    }


    for (; ;)
    {
        fromlen = sizeof(sock_cli); // Tamanho da struct do cliente

        if ((sock_des_cli = accept(socket_des, (struct sockaddr *) &sock_cli, &fromlen)) < 0)
        {
            fprintf(stderr, "Erro de conexao...\n");
            exit(0);
        }
        else
        {
        	proc_filho = fork();
        	if(proc_filho == 0){

        		//Falta declarar as variaveis: operation, option, text 
        		read(sock_des_cli,msg_buffer,sizeof(msg_buffer));
			/*
        		char *token = NULL;

    			token = strtok(msg_buffer, ";");
    			operation = token;

    			token = strtok(NULL, ";");
    			option = token;

    			token = strtok(NULL, ";");
    			text = token;
			*/
		write(sock_des_cli, "Ola vc conectou!", strlen("Ola vc conectou!")+1);

            	pthread_t new_connection_tid;
            	//criar a thread para o usuario passa como argumento o proprio
            	pthread_create(&new_connection_tid, NULL, Options, NULL);

        	}else if(proc_filho > 0){

        	}


            //retirar usuario de usuarios
            //usuarios[i] = null;
        
        }
    }
}

/*strcat(msg_buffer,hora_data());
 strcat(msg_buffer,"</html>\n");
  write(sock_des_cli,msg_buffer,strlen(msg_buffer));
*/

void *Options(void *c)
{
	printf("Cliente conectou!");
	    //close(sock_des_cli);

}
