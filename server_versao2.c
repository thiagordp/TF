#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define CHAVE_MEMORIA_COMPARTILHADA 10
/*
    include das structs no programa cliente, para o mesmo poder ler na memória compartilhada a estrutura de arquivos
*/


void *Options();

/*
	semaforo
*/
sem_t s_cliente;
sem_t* ptr_semaforo;

int sock_des_cli;

int main()
{

/*
	variaveis do socket
*/
    int socket_des, fromlen, tamanho_cliente, proc_filho, mem_id, ptr_mem; //
    struct sockaddr_in servidor, sock_cli;
/*
	variavel mensagem que sera utilizado para escrita no socket
*/    
    char msg_buffer[80];
    int ultimoUser = 0;
/*
	inicializa em 1 o semaforo, verifica se inicializou com sucesso
*/
    if(sem_init(&s_cliente, 1, 1) == -1){
	printf("erro ao inicializar o semaforo \n");
  }	


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

/*
 	Atribuição das características do socket.
*/
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(2000);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(servidor.sin_zero), 8);

/*
 	Relaciona o descritor a referência da struct.
*/
    if (bind(socket_des, (struct sockaddr *) &servidor, sizeof(servidor)) < 0)
    {
	printf("PROBLEMA: %s\n",strerror(errno));
        fprintf(stderr, "Erro de bind\n");
        close(socket_des);  // Fecha
        exit(0);
    }

/*

	Criacao da area de memoria compartilhada
	shmget = criação de uma área de memória compartilhada
	shmat = mapeamento de uma área de memória compartilhada
	
*/    

    mem_id = shmget(CHAVE_MEMORIA_COMPARTILHADA, sizeof(sem_t),0777|IPC_CREAT);
    if(mem_id < 0){
    	printf("Erro ao criar area de memoria compartilhada\n");
    	exit(0);
    }

    ptr_semaforo = (sem_t*)shmat(mem_id, (char*)0, 0);
    if(ptr_semaforo == NULL){
    	printf("Erro de mapeamento de memoria...\n");
    	exit(0);
    }
    *ptr_semaforo = s_cliente;
    printf("Endereco semaforo: %p\n",ptr_semaforo);	

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
        //sem_init(&s_servidor, 0, 1);
    }


    while(1)
    {
        fromlen = sizeof(sock_cli); // Tamanho da struct do cliente

        if ((sock_des_cli = accept(socket_des, (struct sockaddr *) &sock_cli, &fromlen)) < 0)
        {
            fprintf(stderr, "Erro de conexao...\n");
            exit(0);
        }
        else
        {
		printf("------------------------------------------\n");
		printf("conexao aceita\n");
		
        	proc_filho = fork();
        	if(proc_filho == 0){
			printf("processo filho id:%d\n",getpid());
			printf("------------------------------------------\n");
        		//Falta declarar as variaveis: operation, option, text 
        		//read(sock_des_cli,msg_buffer,sizeof(msg_buffer));
			//printf("depois de ler\n");
			/*
        		char *token = NULL;

    			token = strtok(msg_buffer, ";");
    			operation = token;

    			token = strtok(NULL, ";");
    			option = token;

    			token = strtok(NULL, ";");
    			text = token;
			*/
			
			//printf("tenta imprimir\n");
			//printf("%s\n",msg_buffer);
			
			
			printf("servidor escrevendo no socket\n");			
			
			/*
				Servidor escreve no socket
			*/
			int key = 10;
			write(sock_des_cli, key, sizeof(key));
			printf("Finalizada a escrita no socket\n");
			printf("------------------------------------------\n");
			/*read(sock_des_cli, msg_buffer, sizeof(msg_buffer)+1);
			printf("Mensagem recebida: %s\n",msg_buffer);*/

				            	
			pthread_t new_connection_tid;
	            	//criar a thread para o usuario passa como argumento o proprio
	            	pthread_create(&new_connection_tid, NULL, Options, NULL);
			pthread_join(new_connection_tid,NULL);
			pthread_exit(NULL);
			close(sock_des_cli);
        	}else if(proc_filho > 0){

        	}


            //retirar usuario de usuarios
            //usuarios[i] = null;
        
        }
    }
}


void *Options()
{
  sem_t* ptr_mem;
    int mem_id;
    printf("Cliente conectou!\n");
	printf("Endereco semaforo: %p\n",ptr_semaforo);
	printf("------------------------------------------\n");
	printf("Antes do sem_wait\n");
	sem_wait(ptr_semaforo);
	printf("Depois do sem_wait \n");
	printf("------------------------------------------\n");
	/*
		Acesso a sessao critica
	*/
	sleep(10);
	printf("SESSAO CRITICA \n");

	printf("------------------------------------------\n");
	printf("Antes do sem_post \n");		
	sem_post(ptr_semaforo);	
	printf("Depois do sem_post \n");
	printf("------------------------------------------\n");
	while(1);

}
