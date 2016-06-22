//
// Created by thiagordp on 13/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "diretorio.h"
#include "listaPasta.h"
#include "listaArquivo.h"

arquivo_t *criaArq(char nome[], byte dados[], diretorio_t *dirAtual)
{

    // Cria o arquivo
    arquivo_t *novoArquivo = (arquivo_t *) malloc(sizeof(arquivo_t));
    strcpy(novoArquivo->nomeArq, nome);     // Cópia do nome
    strcpy(novoArquivo->dados, dados);      // Cópia dos dados

    // ----------- FAZER VALIDAÇÃO DO NOME E DOS DADOS --------- //

    // Cálculo e atribuição do tamanho do arquivo.
    novoArquivo->tamanho = strlen(novoArquivo->nomeArq) + strlen(novoArquivo->dados);

    // Cria o item que vai na lista de arquivos.
    itemArquivo_t *itemArq = (itemArquivo_t *) malloc(sizeof(itemArquivo_t));
    itemArq->arquivo = novoArquivo;
    itemArq->prevArq = dirAtual->listaArqs;
    itemArq->nextArq = dirAtual->listaArqs->nextArq;

    dirAtual->listaArqs->nextArq = itemArq;

    printf("-----Arquivo-----\n");
    printf("end:\t%p\n", novoArquivo);
    printf("nome:\t%s\n", novoArquivo->nomeArq);
    printf("dados:\t%s\n", novoArquivo->dados);
    printf("tam:\t%ld\n", novoArquivo->tamanho);
}

byte apagaArq(char nome[], diretorio_t *dirAtual)
{

}

arquivo_t *abreArq(char nome[], diretorio_t *dirAtual)
{

}

byte *leArq(char nome[], diretorio_t *dirAtual)
{

}

byte escreveArq(arquivo_t *arquivo, byte dados[])
{

}