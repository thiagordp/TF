//
// Created by thiagordp on 13/06/16.
///*/*
/*
#include <string.h>
#include<stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "filesystem.h"
#include "listaPasta.h"
#include "listaArquivo.h"

#define CHAVE_MEMORIA_ROOT 30
#define CHAVE_MEMORIA_COMPARTILHADA 10
#define CHAVE_MEMORIA_AUTENTICADOR 20
*/
/*
    include das structs no programa cliente, para o mesmo poder ler na memória compartilhada a estrutura de arquivos
*/

// Fazer cópia de verdade dos arquivos

/*
	Tokens para operacoes

char *token = NULL;
char *operation = NULL;
char *option = NULL;
char *text = NULL;

*/
/*
	semaforo
*//*
sem_t semCliente;
sem_t *ptrSemCliente;
*/
/**
 * Semáforo compartilhado entre os processo que acessam o pipe.
 *//*
sem_t semAutenticador;
sem_t *ptrSemAutenticador;

int sock_des_cli;*/

/**
 * Diretório raiz do sistema de arquivos.
 * *//*
diretorio_t *root = NULL;
diretorio_t *ptrRaiz;

int socket_des, fromlen, tamanho_cliente, proc_filho, mem_id, ptr_mem, mem_aut, ptr_aut, mem_raiz; //
struct sockaddr_in servidor, sock_cli;

void ListarConteudo(char *comando)
{
    char *imprimirConteudo;
    imprimirConteudo = (char *) malloc(sizeof(char) * 1024);
    printf("%s\n", comando);
    printf("Operacao de listagem de conteudo\n");
    /*token = strtok(comando, " ");
        operation = token;
        token = strtok(NULL, " ");
        option = token;
        token = strtok(NULL, " ");
        text = token;*//*
    strcat(imprimirConteudo, listaSubDir(ptrRaiz));
    strcat(imprimirConteudo, "\n");
    strcat(imprimirConteudo, listaArquivo(ptrRaiz));
    write(sock_des_cli, imprimirConteudo, MAX_SIZE_BUFFER + 1);

    listaSubDir(ptrRaiz);
    listaArquivo(ptrRaiz);
    free(imprimirConteudo);
}

void CriarDiretorio(char *comando)
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
    criaDir(option, &ptrRaiz);
    write(sock_des_cli, "OK\n", sizeof("OK\n") + 1);

}

void CriarArquivo(char *comando)
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
    printf(" Resultado: %p", criaArq(option, text, ptrRaiz));
    write(sock_des_cli, "OK\n", sizeof("OK\n") + 1);
}

void mostraConteudoArquivo(char *comando)
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
    texto = leArq(option, root);
    printf("%s\n", texto);
    write(sock_des_cli, texto, strlen(texto) + 1);
}

void Remover(char *comando)
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
        printf("Resultado: %d\n", apagaDir(option, root));
    }
    else if (strcmp(text, "arq") == 0)
    {
        printf("Resultado: %d\n", apagaArq(option, root));
    }
    else
    {
        printf("par. invalido\n");
    }
    write(sock_des_cli, "OK\n", sizeof("OK\n") + 1);
    printf("------------------------------------------\n");
    printf("Antes do sem_post \n");
    sem_post(ptrSemCliente);
    printf("Depois do sem_post \n");
    printf("------------------------------------------\n");
}

void Mover(char *comando)
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
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);
    printf(" Foo:   %s\n", foo);

    if (strcmp(foo, "dir") == 0)
    {
        printf("mover diretorio\n");
        // procura pela pasta de destino

        moveDir(option, root, root);
    }
    else if (strcmp(foo, "arq") == 0)
    {
        // procura pela pasta de destino
        printf("mover arquivo\n");
    }
}

void Copiar(char *comando)
{
    printf("Operacao de copiar\n");
    token = strtok(comando, " ");
    operation = token;
    token = strtok(NULL, " ");
    option = token;
    token = strtok(NULL, " ");
    text = token;
    printf(" Operacao: %s\n", operation);
    printf(" Opcao: %s\n", option);
    printf(" Texto: %s\n", text);

    if (strcmp(text, "dir") == 0)
    {
        printf("copiar diretorio\n");
    }
    else if (strcmp(text, "arq") == 0)
    {
        printf("copiar arquivo\n");
    }
}

void Entrar(char *comando)
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

    if (strcmp(text, "..") == 0) // cd ..
    {

    }
    else  // cd nomeDaPasta
    {

    }
}

void ComandoErrado()
{

    write(sock_des_cli, "Comando inexistente, tente novamente\n",
          sizeof("Comando inexistente, tente novamente\n") + 1);


}


long tamanhoFS(diretorio_t *root)
{
    long tamanho = 0;

    itemSubDir_t *item;
    itemArquivo_t *itemArq;

    for (item = root->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        tamanho += tamanhoFS(item->diretorio);
        tamanho += sizeof(itemSubDir_t) + strlen(item->diretorio->nomeDir);
    }

    for (itemArq = root->listaArqs->nextArq; itemArq != NULL; itemArq = itemArq->nextArq)
    {
        tamanho += sizeof(itemArquivo_t) + itemArq->arquivo->tamanho;
    }

    return tamanho;
}

*/