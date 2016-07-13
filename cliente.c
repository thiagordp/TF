#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "util.h"

int main()
{
    char *buffer;
    char buffer_autenticacao[50];
    char usuario[50];
    char *buffer_recebido;
    int len, mysocket, key;
    struct sockaddr_in dest;

    mysocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&dest, 0, sizeof(dest));             /* zero the struct */
    dest.sin_family = AF_INET;

    /* set destination IP number - localhost, 127.0.0.1*/
    dest.sin_addr.s_addr = htonl(INADDR_ANY);

    dest.sin_port = htons(2000);                /* set destination port number *
/*
	Faz a conexao com o servidor
*/
    if (connect(mysocket, (struct sockaddr *) &dest, sizeof(struct sockaddr)) < 0)
    {
        printf("Erro\n");
        exit(0);
    }

/*
	Faz a leitura do que esta no socket
*/
    printf("Entre com o usuario: \n");
    fgets(usuario, 50, stdin);
    usuario[strlen(usuario)-1]='\0';
    write(mysocket,usuario, strlen(usuario) + 1);
    read(mysocket, buffer_autenticacao, sizeof(buffer_autenticacao) + 1);
    printf("escreveu no socket: %s\n", buffer_autenticacao);

    if (strcmp(buffer_autenticacao, "V") == 0)
    {
        read(mysocket, buffer_autenticacao, sizeof(buffer_autenticacao) + 1);
        printf(" %s\n", buffer_autenticacao);
        buffer = NULL;
	buffer_recebido = NULL;

        do
        {
            free(buffer);
            buffer = (char *) calloc(50, sizeof(char));
	    buffer_recebido = (char *) calloc(MAX_SIZE_BUFFER, sizeof(char));
            //scanf("%s",buffer);
            fgets(buffer, 50, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            printf("cmd: |%s|\n", buffer);

            write(mysocket, buffer, strlen(buffer) + 1);
            read(mysocket,buffer_recebido,MAX_SIZE_BUFFER + 1);
            printf(" %s\n",buffer_recebido);
	    
        }
        while (strcmp(buffer, "exit") != 0);
    }
    else
    {
        printf(" %s\n", buffer_recebido);
    }
}
