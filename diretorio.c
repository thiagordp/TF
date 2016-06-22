//
// Created by thiagordp on 13/06/16.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "diretorio.h"
#include "listaArquivo.h"
#include "listaPasta.h"


int criaDir(char nome[], diretorio_t **dirAtual)
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

    // Ao invés disso chamar o criar arquivo
    newListaArq->arquivo = NULL;
    newListaArq->nextArq = newListaArq->prevArq = NULL;
    novoDiretorio->listaArqs = newListaArq;

    // Criação da lista de subdiretórios
    novoDiretorio->listasubDirs = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));
    novoDiretorio->listasubDirs->diretorio = NULL;
    novoDiretorio->listasubDirs->nextDir = novoDiretorio->listasubDirs->prevDir = NULL;

    //
    printf("-----Diretorio-----\n");
    printf("end:\t%p\n", novoDiretorio);
    printf("nome:\t%s\n", novoDiretorio->nomeDir);
    printf("arq:\t%p\n", novoDiretorio->listaArqs);
    printf("dir:\t%p\n", novoDiretorio->listasubDirs);

    // --------- Inserção do novo diretório como subDiretório do dirAtual. ---------//

    // Caso o diretório não exista ainda.
    if (*dirAtual == NULL)
    {
        *dirAtual = novoDiretorio; // O diretório criado é o root.
    }

        // Caso a lista de subdiretório esteja vazia.
    else if ((*dirAtual)->listasubDirs == NULL)
    {

    }

        // Caso tudo já tenha sido criado pelo menos uma vez.
    else
    {
        itemSubDir_t *novoDir = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));
        novoDir->diretorio = novoDiretorio;
        novoDir->prevDir = (*dirAtual)->listasubDirs;
        novoDir->nextDir = (*dirAtual)->listasubDirs->nextDir;
        (*dirAtual)->listasubDirs->nextDir = novoDir;
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