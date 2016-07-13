//
// Created by thiagordp on 13/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "diretorio.h"
#include "listaArquivo.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
arquivo_t *criaArq(char nome[], byte dados[], diretorio_t *dirAtual)
{
    if (strlen(nome) == 0 || strlen(dados) == 0 || dirAtual == NULL)
    {
        return NULL;
    }


    // Cria o arquivo
    arquivo_t *novoArquivo = (arquivo_t *) malloc(sizeof(arquivo_t));
    strcpy(novoArquivo->nomeArq, nome);     // Cópia do nome
    strcpy(novoArquivo->dados, dados);      // Cópia dos dados

    // ----------- FAZER VALIDAÇÃO DO NOME E DOS DADOS --------- //

    // Cálculo e atribuição do tamanho do arquivo.
    novoArquivo->tamanho = sizeArq(novoArquivo);

    // Cria o item que vai na lista de arquivos.
    itemArquivo_t *itemArq = (itemArquivo_t *) malloc(sizeof(itemArquivo_t));
    itemArq->arquivo = novoArquivo;
    itemArq->prevArq = dirAtual->listaArqs;
    itemArq->nextArq = dirAtual->listaArqs->nextArq;

    // Colocar o ponteiro 'prev' do primeiro arquivo, se houver, para o novo.
    if (dirAtual->listaArqs->nextArq != NULL)
    {
        dirAtual->listaArqs->nextArq->prevArq = itemArq;
    }
    dirAtual->listaArqs->nextArq = itemArq;

    dirAtual->count_arq++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short apagaArq(char nome[], diretorio_t *dirAtual)
{
    itemArquivo_t *itemArq = NULL;

    arquivo_t *arq = abreArq(nome, dirAtual, &itemArq);

    if (arq == NULL || itemArq == NULL)
    {
        return ER_NULL_POINTER;
    }

    // Mexer os ponteiros
    itemArq->prevArq->nextArq = itemArq->nextArq;

    if (itemArq->nextArq != NULL)
    {
        itemArq->nextArq->prevArq = itemArq->prevArq;
    }

    dirAtual->count_arq--;

    // Free nas variáveis
    free(arq);
    free(itemArq);

    return EXIT_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arquivo_t *abreArq(char nome[], diretorio_t *dirAtual, itemArquivo_t **item)
{
    arquivo_t *arq = NULL;

    if (strlen(nome) == 0)
    {
        return arq;
    }

    for (*item = dirAtual->listaArqs->nextArq;
         *item != NULL;
         *item = (*item)->nextArq)
    {
        if (strcmp(nome, (*item)->arquivo->nomeArq) == 0)
        {
            arq = (*item)->arquivo;

            printf("item:\t%p\narq:\t%p\n", item, arq);
            break;
        }
    }

    return arq;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byte *leArq(char nome[], diretorio_t *dirAtual)
{
    itemArquivo_t *item;
    arquivo_t *arq = NULL;

    for (item = dirAtual->listaArqs->nextArq; item != NULL; item = item->nextArq)
    {
        if (strcmp(item->arquivo->nomeArq, nome) == 0)
        {
            arq = item->arquivo;
        }
    }

    if (arq != NULL)
    {
        byte *dados = calloc(strlen(arq->dados), sizeof(byte));
        strcpy(dados, arq->dados);

        return dados;
    }

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short escreveArq(arquivo_t *arq, byte dados[])
{
    if (arq == NULL)
    {
        return ER_NULL_POINTER;
    }
    else if (strlen(dados) > MAX_SIZE_ARQ)
    {
        return ER_INVALID_STRING;
    }

    strcpy(arq->dados, dados);

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short renomeiaArq(char nome[], arquivo_t *arq)
{
    if (arq == NULL)
    {
        return ER_NULL_POINTER;
    }
    else if (strlen(nome) == 0)
    {
        return ER_EMPTY_STRING;
    }
    else if (strlen(nome) > MAX_SIZE_NAME_ARQ)
    {
        return ER_INVALID_STRING;
    }

    strcpy(arq->nomeArq, nome);

    arq->tamanho = sizeArq(arq);

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

size_data_t sizeArq(arquivo_t *arq)
{
    return (strlen(arq->dados) + strlen(arq->nomeArq));
}

/*
arquivo_t *varreArquivo(char nome[], diretorio_t *root)
{

    // Rever lógica, pq pode ter mais de um arquivo com o mesmo nome,
    // mais em dir diferentes.
    if (root == NULL || arq == NULL)
    {
        return NULL;
    }
    else if (strlen(nome) == 0)
    {
        return NULL;
    }

    itemSubDir_t *itemDir, *tempDir;
    itemArquivo_t *itemArq, *tempArq;

    for (itemArq = root->listaArqs->nextArq; itemArq != NULL; itemArq = item->nextArq)
    {
        if (strcmp(itemArq->arquivo->nomeArq, nome) == 0)
        {
            return itemArq->arquivo;
        }
    }

    for (itemDir = root->listasubDirs->nextDir; item != NULL; itemDir = itemDir->nextDir)
    {
        for (itemArq = root->listaArqs->nextArq; itemArq != NULL; itemArq = item->nextArq)
        {
            if (strcmp(itemArq->arquivo->nomeArq, nome) == 0)
            {

            }
        }
    }
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int moveArquivo(char nome[], diretorio_t *dirOrigem, diretorio_t *dirDestino)
{
    // Verificação de erros
    if (dirOrigem == NULL || dirDestino == NULL)
    {
        return ER_NULL_POINTER;
    }
    else if (strlen(nome) == 0)
    {
        return ER_EMPTY_STRING;
    }

    itemArquivo_t *item;

    for (item = dirOrigem->listaArqs->nextArq; item != NULL; item = item->nextArq)
    {
        if (strcmp(nome, item->arquivo->nomeArq) == 0)
        {
            break;
        }
    }

    if (item == NULL)
    {
        return ER_NOT_FOUND;
    }

    item->prevArq->nextArq = item->nextArq;
    if (item->nextArq != NULL)
    {
        item->nextArq->prevArq = item->prevArq;
    }

    item->prevArq = dirDestino->listaArqs;
    item->nextArq = dirDestino->listaArqs->nextArq;

    dirDestino->listaArqs->nextArq = item;
    if (item->nextArq != NULL)
    {
        item->nextArq->prevArq = item;
    }

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copiaInfoArquivo(arquivo_t *arqOrigem, arquivo_t **arqDestino)
{

}


int copyArq(arquivo_t *arqDestino, arquivo_t *arqOrigem, diretorio_t *dir)
{

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copiaArquivo(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino)
{
    arquivo_t *arqOrigem, *arqDestino;

    // Procurar o arquivo na pasta
    // Pegar o ponteiro
    arqOrigem = procuraArq(nomeDir, dirOrigem);

    if (arqOrigem == NULL)
    {
        return ER_NOT_FOUND;
    }

    criaArq(arqOrigem->nomeArq, arqOrigem->dados, dirDestino);

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

arquivo_t *procuraArq(char nomeArq[], diretorio_t *dirAtual)
{
    arquivo_t *arq = NULL;

    for (itemArquivo_t *item = dirAtual->listaArqs->nextArq; item != NULL; item = item->nextArq)
    {
        if (strcmp(item->arquivo->nomeArq, nomeArq) == 0)
        {
            arq = item->arquivo;
            break;
        }
    }

    return arq;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////