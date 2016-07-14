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
//#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include "arquivo.h"
#include "diretorio.h"
#include "usuario.h"
#include "util.h"

#define CHAVE_MEMORIA_COMPARTILHADA 10
#define CHAVE_MEMORIA_AUTENTICADOR 20

/*
    include das structs no programa cliente, para o mesmo poder ler na memória compartilhada a estrutura de arquivos
*/


void *Options(void *opt);

//void *Autenticacao(void *aut);
void Autenticacao(int sock, usuario_t **user);

void *TrataConexao(void *sockDes);

void ListarArquivos(char *comando);

void ListarConteudo(char *comando, int sock, usuario_t *user);

void CriarDiretorio(char *comando, int sock, usuario_t *user);

void CriarArquivo(char *comando, int sock, usuario_t *user);

void mostraConteudoArquivo(char *comando, int sock, usuario_t *user);

void Remover(char *comando, int sock, usuario_t *user);

void Mover(char *comando, usuario_t *user);

void Copiar(char *comando, usuario_t *user);

void Entrar(char *comando, usuario_t *user);

void *NovoCliente(void *c);

void ComandoErrado(int sock);

/*
	Tokens para operacoes
*/
char *token = NULL;
char *operation = NULL;
char *option = NULL;
char *text = NULL;

//usuario_t *user;

/*
	semaforo
*/
sem_t semCliente;
sem_t *ptrSemCliente;

/**
 * Semáforo compartilhado entre os processo que acessam o pipe.
 */
sem_t semAutenticador;
sem_t *ptrSemAutenticador;

int sock_des_cli;

/**
 * Diretório raiz do sistema de arquivos.
 * */
diretorio_t *root = NULL;
//diretorio_t *ptrRaiz;

int main()
{

/*
	variaveis do socket
ptr*/
    int socket_des, *newSock, fromlen, tamanho_cliente, proc_filho, mem_id, ptr_mem, mem_aut, ptr_aut; //
    struct sockaddr_in servidor, sock_cli;
/*
	variavel mensagem que sera utilizado para escrita no socket
*/
    //int ultimoUser = 0;
/*
	inicializa em 1 o semaforo, verifica se inicializou com sucesso
*/
    if (sem_init(&semCliente, 1, 1) == -1)
    {
        printf("erro ao inicializar o semaforo \n");
    }

    if (sem_init(&semAutenticador, 1, 1) < 0)
    {
        printf("Erro ao inicializar o semáforo do autenticador.");
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

    ptrSemCliente = (sem_t *) shmat(mem_id, (char *) 0, 0);
    if (ptrSemCliente == NULL)
    {
        printf("Erro de mapeamento de memoria...\n");
        exit(0);
    }
    *ptrSemCliente = semCliente;
    printf("Endereco semaforo: %p\n", ptrSemCliente);


    /**
     * Criação da memória compartilhada que será usada para controlar o acesso ao pipe do autenticador.
     * */
    mem_aut = shmget(CHAVE_MEMORIA_AUTENTICADOR, sizeof(sem_t), 0777 | IPC_CREAT);

    if (mem_aut < 0)
    {
        printf("Erro ao criar memória criada para o autenticador");
        exit(0);
    }

    ptrSemAutenticador = (sem_t *) shmat(mem_id, (char *) 0, 0);

    if (ptrSemAutenticador == NULL)
    {
        printf("Erro no mapeamento de memória...\n");
        exit(0);
    }

    *ptrSemAutenticador = semAutenticador;
    printf("Endereço do semáforo: %p\n\n", ptrSemAutenticador);


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

/**
* Criação do diretório raiz do sistema.
*/
    criaDir("root", &root);

    printf("Endereco do diretorio raiz %p\n", root);
    while (1)
    {/*
        listaSubDir(ptrRaiz);
        listaArquivo(ptrRaiz);*/
        printf("SERVER: A espera de conexões...\n");

        fromlen = sizeof(sock_cli); // Tamanho da struct do cliente
        if ((sock_des_cli = accept(socket_des, (struct sockaddr *) &sock_cli,
                                   (socklen_t *) &fromlen)) < 0)
        {
            fprintf(stderr, "Erro de conexao...\n");
            exit(0);
        }

        printf("------------------------------------------\n");
        printf("conexao aceita\n");

        newSock = (int *) malloc(sizeof(int));

        *newSock = sock_des_cli;
        printf("sock: %d\t%d\n", *newSock, sock_des_cli);
        pthread_t new_connection_tid;
        pthread_create(&new_connection_tid, NULL, TrataConexao, (void *) newSock);

        // pthread_join(new_connection_tid, NULL);
        /*proc_filho = fork();
        printf("Socket: %d", sock_cli);
        if (proc_filho == 0)
        {
            printf("processo filho id:%d\n", getpid());
            printf("ENDERECO RAIZ %p\n", ptrRaiz);
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
        }*/
    }
}

void Autenticacao(int sock, usuario_t **user)
{
    printf("SERVER: CHEGOU NA THREAD AUTENTICACAO \n");
    printf("sock: %d\n", sock);
    char msg_buffer[80];
    char usuario[30];
    int fd_server, fd_autenticador, pid;

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
    read(sock, usuario, sizeof(usuario) + 1);
    printf("Usuario conectado: %s\n", usuario);



    /*
        Escreve no PIPE o usuario passado pelo cliente
    */
    sem_wait(&semAutenticador);
    write(fd_autenticador, usuario, sizeof(usuario) + 1);
    bzero(msg_buffer, sizeof(msg_buffer));

    printf("SERVER: Lendo a resposta da autenticação...\n");
    read(fd_server, msg_buffer, sizeof(msg_buffer));
    printf("SERVER: Mensagem recebida do autenticador...: |%s|\n", msg_buffer);

    sem_post(&semAutenticador);
    if (strcmp(msg_buffer, "V") == 0)
    {
        printf("SERVER: Validado\n");
        write(sock, "V", sizeof("V") + 1);

        // criação do usuário
        *user = criaUsuario(usuario, root);
    }
    else
    {
        printf("SERVER: User invalido.\n");
        write(sock, "Usuario invalido, tente novamente\n",
              sizeof("Usuario invalido, tente novamente\n") + 1);
        close(sock);
        exit(0);
    }

    close(fd_server);
    close(fd_autenticador);
}

void *Options(void *opt)
{
    int sock = *(int *) opt;
    usuario_t *user = NULL;

    char comando[50];
    printf("SERVER: CHEGOU NA THREAD VERIFICACAO \n");

    /*
        Cria a thread de autenticacao
    */
    /*  pthread_t autenticacao_tid;
      pthread_create(&autenticacao_tid, NULL, Autenticacao, array);
      pthread_join(autenticacao_tid, NULL);
  */

    Autenticacao(sock, &user);

    if (user == NULL)
    {

        printf("Nous avons un problème");
        pthread_exit(NULL);
    }

    sem_t *ptr_mem;

    printf("Cliente conectou!\n");
    printf("Endereco semaforo: %p\n", ptrSemCliente);

    /*
        Acesso a sessao critica
    */

    bzero(&sock, 0);
    /////////////////////////// PROBLEMA   /////////////////////////////////////
    write(sock, "Conexao realizada com sucesso",
          sizeof("Conexao realizada com sucesso") + 1);

    printf("SESSAO CRITICA \n");

    sleep(1);

    do
    {
        printf("reading...\n");
        read(sock, comando, sizeof(comando));
        printf("Comando recebido:|%s|\n", comando);

        char *comando_original = malloc(sizeof(char) * 30);
        strcpy(comando_original, comando);

        comando;
        token = strtok(comando, " ");
        operation = token;

        if (strcmp(operation, "ls") == 0)
        {
            //ListarConteudo(comando_original, sock);
            ListarConteudo(comando_original, sock, user);
            //listaArquivo(root);
        }
        else if (strcmp(operation, "mkdir") == 0)
        {
            CriarDiretorio(comando_original, sock, user);
        }
        else if (strcmp(operation, "touch") == 0)
        {
            printf("antes de iniciar o touch\n");
            CriarArquivo(comando_original, sock, user);
            printf("depois do touch\n");
        }
        else if (strcmp(operation, "cat") == 0)
        {
            mostraConteudoArquivo(comando_original, sock, user);
        }
        else if (strcmp(operation, "rm") == 0)
        {
            Remover(comando_original, sock, user);
        }
        else if (strcmp(operation, "mv") == 0)
        {
            Mover(comando_original, user);
        }
        else if (strcmp(operation, "cp") == 0)
        {
            Copiar(comando_original, user);
        }
        else if (strcmp(operation, "cd") == 0)
        {
            Entrar(comando_original, user);
        }
        else if (strcmp(operation, "clear") == 0)
        {
            system("clear");
        }
        else if (strcmp(operation, "exit") != 0)
        {
            ComandoErrado(sock);
        }

        free(comando_original);
        bzero(&sock, 0);
    }
    while (strcmp(comando, "exit") != 0);

    printf("fim da execucao do cliente\n");
}

void *NovoCliente(void *c)
{
    pthread_t new_connection_tid;
    pthread_create(&new_connection_tid, NULL, Options, NULL);
    pthread_join(new_connection_tid, NULL);
}

//void *TrataConexao(void *sockDes)
void *TrataConexao(void *sockDes)
{
    pthread_t new_connection_tid;
    pthread_create(&new_connection_tid, NULL, Options, sockDes);
    pthread_join(new_connection_tid, NULL);
}

void ListarConteudo(char *comando, int sock, usuario_t *user)
{
    char *imprimirConteudo;
    imprimirConteudo = (char *) malloc(sizeof(char) * 1024);
    printf("%s\n", comando);
    printf("Operacao de listagem de conteudo\n");

    strcat(imprimirConteudo, listaSubDir(user->dirAtual->diretorio));
    strcat(imprimirConteudo, "\n");
    strcat(imprimirConteudo, listaArquivo(user->dirAtual->diretorio));

    write(sock, imprimirConteudo, MAX_SIZE_BUFFER + 1);

    /*  listaSubDir(ptrRoot);
      listaArquivo(ptrRoot);*/

    free(imprimirConteudo);
}

void CriarDiretorio(char *comando, int sock, usuario_t *user)
{

    printf("Operacao de criar diretorio\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    printf("US: %p\n", user);
    printf("CRIA: %s\t%p\n", option, user->dirAtual->diretorio);

    criaDir(option, &user->dirAtual->diretorio);

    write(sock, "OK\n", sizeof("OK\n") + 1);

}

void CriarArquivo(char *comando, int sock, usuario_t *user)
{
    printf("Operacao de criar arquivo\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;

    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    printf(" Resultado: %p", criaArq(option, text, user->dirAtual->diretorio));
    write(sock, "OK\n", sizeof("OK\n") + 1);
}

void mostraConteudoArquivo(char *comando, int sock, usuario_t *user)
{
    printf("Operacao de mostrar conteudo do arquivo\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    char *texto;
    texto = leArq(option, user->dirAtual->diretorio);
    printf("%s\n", texto);
    write(sock, texto, strlen(texto) + 1);
}

void Remover(char *comando, int sock, usuario_t *user)
{
    printf("------------------------------------------\n");
    printf("Antes do sem_wait\n");

    sem_wait(ptrSemCliente);

    printf("Depois do sem_wait \n");
    printf("------------------------------------------\n");
    printf("Operacao de remocao\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    sleep(10);

    if (strcmp(text, "dir") == 0)
    {
        printf("Resultado: %d\n", apagaDir(option, user->dirAtual->diretorio));
    }
    else if (strcmp(text, "arq") == 0)
    {
        printf("Resultado: %d\n", apagaArq(option, user->dirAtual->diretorio));
    }
    else
    {
        printf("par. invalido\n");
    }

    write(sock, "OK\n", sizeof("OK\n") + 1);
    printf("------------------------------------------\n");
    printf("Antes do sem_post \n");
    sem_post(ptrSemCliente);
    printf("Depois do sem_post \n");
    printf("------------------------------------------\n");
}

void Mover(char *comando, usuario_t *user)
{
    char *foo;

    printf("Operacao de mover\n");

    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    token = strtok(NULL, " ");
    foo = token;

    printf(" Operacao: %s\n", operation);
    printf(" Opcao:    %s\n", option);
    printf(" Texto:    %s\n", text);
    printf(" Foo:      %s\n", foo);

    char **tokens = NULL;
    size_t x;
    tokenize(text, "/", &tokens, &x);

    for (size_t i = 0; i < x; i++)
    {
        printf("|%s|\n", tokens[i]);
    }

    diretorio_t *dir = procuraDiretorioRec(tokens, x, root);

    // mv arquivo destino dir
    if (strcmp(foo, "dir") == 0)
    {
        printf("mover diretorio\n");
        // procura pela pasta de destino

        moveDir(option, user->dirAtual->diretorio, dir);
        // procurar o end. de destino
        // copiar...
    }
    else if (strcmp(foo, "arq") == 0)
    {
        // procurar o end. de destino
        // copiar...

        // procura pela pasta de destino
        printf("mover arquivo\n");
        moveArquivo(option, user->dirAtual->diretorio, dir);
    }
}

void Copiar(char *comando, usuario_t *user)
{
    char *pt = NULL;

    printf("Operacao de copiar\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    token = strtok(NULL, " ");
    pt = token;

    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    printf(" Tipo:  %s\n", pt);
    // cp arquivo.txt root/dir1/dir2
    // cp arquivo.txt root/dir1/dir2

    char **tokens = NULL;
    int x;
    tokenize(text, "/", &tokens, &x);

    for (int i = 0; i < x; i++)
    {
        printf("|%s|\n", tokens[i]);
    }

    diretorio_t *dir = procuraDiretorioRec(tokens, x, root);

    if (strcmp(pt, "dir") == 0)
    {
        printf("copiar diretorio\n");

        copyDir(option, user->dirAtual->diretorio, dir);
        // procurar o end. de destino
        // copiar...
    }
    else if (strcmp(pt, "arq") == 0)
    {
        // procurar o end. de destino
        // copiar...

        printf("copiar arquivo\n");

        copiaArquivo(option, user->dirAtual->diretorio, dir);
    }
}

void Entrar(char *comando, usuario_t *user)
{
    printf("Operação de entrar");

    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);

    if (strcmp(option, "..") == 0) // cd ..
    {
        retrocedeDir(user);
    }
    else
    {
        avancaDir(user, procuraDiretorio(option, user->dirAtual->diretorio));
    }
}

void ComandoErrado(int sock)
{
    write(sock, "Comando inexistente, tente novamente\n",
          sizeof("Comando inexistente, tente novamente\n") + 1);
}

