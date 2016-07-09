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
#include <sys/stat.h>
#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>

#define CHAVE_MEMORIA_COMPARTILHADA 10

/*
    include das structs no programa cliente, para o mesmo poder ler na memória compartilhada a estrutura de arquivos
*/


void *Options(void *);

void *Autenticacao(void *);

void *Verificacao(void *);

/*
	semaforo
*/
sem_t s_cliente;
sem_t *ptr_semaforo;

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
    int ultimoUser = 0;
/*
	inicializa em 1 o semaforo, verifica se inicializou com sucesso
*/
    if (sem_init(&s_cliente, 1, 1) == -1)
    {
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
    int x = 1;

    // Sobreescrever quem estiver usando o IP e porta que serão utilizados no código.
    if (setsockopt(socket_des, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(int)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");

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
        printf("PROBLEMA: %s\n", strerror(errno));
        fprintf(stderr, "Erro de bind\n");
        close(socket_des);  // Fecha
        exit(0);
    }

/*
	Criacao da area de memoria compartilhada
	shmget = criação de uma área de memória compartilhada
	shmat = mapeamento de uma área de memória compartilhada

*/

    mem_id = shmget(CHAVE_MEMORIA_COMPARTILHADA, sizeof(sem_t), 0777 | IPC_CREAT);
    if (mem_id < 0)
    {
        printf("Erro ao criar area de memoria compartilhada\n");
        exit(0);
    }

    ptr_semaforo = (sem_t *) shmat(mem_id, (char *) 0, 0);
    if (ptr_semaforo == NULL)
    {
        printf("Erro de mapeamento de memoria...\n");
        exit(0);
    }
    *ptr_semaforo = s_cliente;
    printf("Endereco semaforo: %p\n", ptr_semaforo);

/*

		Habilita o serv. para receber até 10 conexões.
     	socket_des - socket
     	10 - nº máximo de conexões.
*/
    if (listen(socket_des, 10) < 0)
    {
        fprintf(stderr, "Erro de listen\n");
        exit(0);
    }

    printf("Servidor pronto para conexoes...\n");
    // sem_init(&s_servidor, 0, 1);

    printf("%d\t", unlink("pipe.servidor"));
    printf("%d\n", unlink("pipe.autenticador"));

    if (mkfifo("pipe.servidor", 0777) < 0)
    {
        printf("Erro ao criar pipe do servidor...\n");
        exit(0);
    }

    if (mkfifo("pipe.autenticador", 0777) < 0)
    {
        printf("Erro ao criar pipe do cliente...\n");
        exit(0);
    }

    printf("pipe criado\n");

    int pid = fork();

    if (pid < 0)
    {
        printf("SERVER: erro no fork");
        exit(0);
    }
    else if (pid == 0)
    {
        printf("SERVER: filho\n");
        printf("SERVER: Exec %d\n", execlp("./user_db", "./user_db", NULL));
        printf("SERVER: depois exec\n");
    }

    printf("SERVER: iniciando...\n");

    while (1)
    {
        printf("A espera de conexões...\n");

        fromlen = sizeof(sock_cli); // Tamanho da struct do cliente

        if ((sock_des_cli = accept(socket_des, (struct sockaddr *) &sock_cli,
                                   (socklen_t *) &fromlen)) < 0)
        {
            fprintf(stderr, "Erro de conexao...\n");
            exit(0);
        }

        printf("------------------------------------------\n");
        printf("conexao aceita\n");

        proc_filho = fork();
        if (proc_filho == 0)
        {
            printf("processo filho id:%d\n", getpid());

            pthread_t new_connection_tid;
            pthread_create(&new_connection_tid, NULL, Options, NULL);
            pthread_join(new_connection_tid, NULL);

            close(sock_des_cli);
            pthread_exit(NULL);
        }
        else if (proc_filho > 0)
        {
            printf("processo pai: %d\n", getpid());
        }
        else
        {
            exit(0);
        }
    }
}

void *Autenticacao(void *)
{

    printf("CHEGOU NA THREAD AUTENTICACAO \n");

    char msg_buffer[80];
    char usuario[30];
    int fd_server, fd_autenticador, pid;

    // else -- continua a processa.

    /*
        Abertura do PIPE
    */
    printf("Abrindo o pipe do servidor para leitura...\n");
    fd_server = open("pipe.servidor", O_RDONLY);
    printf("Abrindo o pipe do autenticador para escrita...\n");
    fd_autenticador = open("pipe.autenticador", O_WRONLY);

    if (fd_server < 0)
    {
        printf("Erro ao abrir PIPE servidor!\n");
        exit(0);
    }

    if (fd_autenticador < 0)
    {
        printf("Erro ao abrir PIPE cliente!\n");
        exit(0);
    }

    printf("Pipes nomeados criados com sucesso!\n");
    printf("------------------------------------------\n");

    /*
        Fazer a leitura do usuario que foi escrito no socket pelo cliente
    */

    printf("Lendo do socket...\n");
    read(sock_des_cli, usuario, sizeof(usuario) + 1);
    printf("Usuario conectado: %s\n", usuario);

    /*
        Escreve no PIPE o usuario passado pelo cliente
    */
    write(fd_autenticador, usuario, sizeof(usuario) + 1);
    //write(fd_autenticador, "", sizeof("") + 1);
    bzero(msg_buffer, sizeof(msg_buffer));

    printf("SERVER: Lendo a resposta da autenticação...\n");
    read(fd_server, msg_buffer, sizeof(msg_buffer));
    printf("SERVER: Mensagem recebida do autenticador...: |%s|\n", msg_buffer);

    if (strcmp(msg_buffer, "V") == 0)
    {
        printf("SERVER: Validado\n");
    }
    else
    {
        printf("SERVER: User invalido.\n");
    }

    close(fd_server);
    close(fd_autenticador);

    pthread_exit(NULL);
}

void *Options(void *)
{
    printf("SERVER: CHEGOU NA THREAD VERIFICACAO \n");

    /*
        Cria a thread de autenticacao
    */
    pthread_t autenticacao_tid;
    pthread_create(&autenticacao_tid, NULL, Autenticacao, NULL);
    pthread_join(autenticacao_tid, NULL);

    sem_t *ptr_mem;
    int mem_id;
    printf("Cliente conectou!\n");
    printf("Endereco semaforo: %p\n", ptr_semaforo);
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

    while (1)
    { }
}