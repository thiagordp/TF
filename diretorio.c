//
// Created by thiagordp on 13/06/16.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "diretorio.h"
#include "listaArquivo.h"
#include "listaPasta.h"

int criaDir(char nome[], diretorio_t *dirAtual)
{
    // Decl. do ponteiro pro novo diretório
    diretorio_t *novoDiretorio;

    // Aloca dinamicamente
    novoDiretorio = (diretorio_t *) malloc(sizeof(diretorio_t));

    // Copia o nome para a estrutura do diretório.
    strcpy(novoDiretorio->nomeDir, nome);

    // Cria a cabeça da listade arquivos da nova pasta
    itemArquivo_t *newListaArq = (itemArquivo_t *) malloc(sizeof(itemArquivo_t));

    newListaArq->arquivo = NULL;

    // ao invés disso chamar o criar arquivo
    newListaArq->arquivo = NULL;
    newListaArq->nextArq = newListaArq->prevArq = NULL;
    novoDiretorio->listaArqs = newListaArq;

    novoDiretorio->listasubDirs = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));
    novoDiretorio->listasubDirs->diretorio = NULL;
    novoDiretorio->listasubDirs->nextDir = novoDiretorio->listasubDirs->prevDir = NULL;

    ///////////////////////////////////////////////////////////////////////////
    itemSubDir_t *newListaDir = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));
    newListaDir->nextDir = newListaDir->prevDir = NULL;
    newListaDir->diretorio = NULL;

    // Inserção do novo diretório como subDiretório do dirAtual.
    if (dirAtual == NULL)
    {

    }
    else if (dirAtual->listasubDirs == NULL)
    {
        dirAtual->listasubDirs = newListaDir;
    }
    else
    {
        newListaDir->nextDir = dirAtual->listasubDirs->nextDir;

        newListaDir->prevDir = dirAtual->listasubDirs;

        dirAtual->listasubDirs->prevDir = NULL;
        printf("Start\n");
    }
}

int apagaDir(char nome[], diretorio_t *dirAtual)
{

}

int listaSubDir(diretorio_t *dirAtual)
{

}

int renomeiaDir(diretorio_t *dirAtual)
{

}