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

    // --------- Inserção do novo diretório como subDiretório do dirAtual. ---------//

    // Caso o diretório não exista ainda.
    if (*dirAtual == NULL)
    {
        *dirAtual = novoDiretorio; // O diretório criado é o root.

        /*
        printf("-----Diretorio-----\n");
        printf("end:\t%p\n", novoDiretorio);
        printf("nome:\t%s\n", novoDiretorio->nomeDir);
        printf("arq:\t%p\n", novoDiretorio->listaArqs);
        printf("dir:\t%p\n", novoDiretorio->listasubDirs);
        */
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

        if (novoDir->nextDir != NULL)
        {
            novoDir->nextDir->prevDir = novoDir;
        }

        (*dirAtual)->listasubDirs->nextDir = novoDir;

        printf("---subdir----\nnome:\t%p\nprev:\t%p\nnext:\t%p\n-------------\n",
               novoDiretorio->nomeDir,
               novoDir->prevDir,
               novoDir->nextDir
        );
    }
}

int apagaDir(char nome[], diretorio_t *dirAtual)
{
    if (dirAtual == NULL)
    {
        printf("ER_NULL_POINTER\n");
        return ER_NULL_POINTER;
    }

    // se o nome for vazio, apaga tudo o que tem dentro do diretório.
    if (strlen(nome) == 0)
    {
        //if (dirAtual->count_arq > 0) // REVISAR: talvez tenha que passar de qualquer forma
        //{

        printf("LEN = 0\n");

        itemArquivo_t *item, *tempItem;
        arquivo_t *tempArq;

        item = dirAtual->listaArqs;

        while (item != NULL)
        {
            // Guarda os ponteiros para poder liberar da memória.
            tempItem = item;
            tempArq = item->arquivo;

            // Aponta para o próximo.
            item = item->nextArq;

            // Liberação da memória.
            free(tempArq);
            free(tempItem);
        }

        // free(dirAtual->listaArqs);

        // dirAtual->listaArqs = NULL;
        //}
        //if (dirAtual->count_dir > 0) // REVISAR: talvez tenha que passar de qualquer forma
        //{
        itemSubDir_t *itemDir, *tempDir;
        diretorio_t *dir;

        for (itemDir = dirAtual->listasubDirs;
             itemDir != NULL;
            /* vazio */)
        {
            dir = itemDir->diretorio;

            apagaDir("", dir);

            tempDir = itemDir;
            itemDir = itemDir->nextDir;

            free(dir);
            free(tempDir);
        }
        //}
    }
    else
    {
        printf("LEN > 0\n");
        diretorio_t *dirAt;
        itemSubDir_t *item;

        for (item = dirAtual->listasubDirs->nextDir;
             item != NULL && item->diretorio != NULL &&
             strcmp(item->diretorio->nomeDir, nome) != 0;
             item = item->nextDir);

        if (item == NULL || item->diretorio == NULL)
        {
            printf("NULL / LEN > 0");
            return ER_NULL_POINTER;
        }

        // mexer nos ponteiros da lista de subdiretórios.
        item->prevDir->nextDir = item->nextDir;
        if (item->nextDir != NULL)
        {
            item->nextDir->prevDir = item->prevDir;
        }

        dirAt = item->diretorio;

        // Apaga todos os arquivos e pastas que existirem dentro desse diretório.
        apagaDir("", dirAt);
        free(dirAt);
        free(item);
    }
    // caso contrário, apaga só o diretório com esse nome.
}

int listaSubDir(diretorio_t *dirAtual)
{
    printf("---------------------------------\n> %s\n---------------------------------\n",
           dirAtual->nomeDir);
    printf("Nome\t\t\tNo Arqs\t\tNo Dirs\t\t\n");

    itemSubDir_t *item;
    diretorio_t *dir;

    for (item = dirAtual->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        dir = item->diretorio;

        printf("%s\t\t\t%ld\t\t%ld\n", dir->nomeDir, dir->count_arq, dir->count_dir);
    }
}

int listaArquivo(diretorio_t *dirAtual)
{

}

int renomeiaDir(char nome[], diretorio_t *dirAtual)
{
    if (dirAtual == NULL)
    {
        return -2;
    }
    else if (strlen(nome) == 0)
    {
        return -1;
    }
    else if (strlen(nome) > MAX_SIZE_NAME_ARQ)
    { // Corrigir o tamnho
        return 0;
    }

    strcpy(dirAtual->nomeDir, nome);

    return 1;
}

diretorio_t *procuraArquivo(char nome[], diretorio_t *dirAtual)
{
    itemSubDir_t *item;
    printf("\nSTART SEARCHING\n");

    if (dirAtual == NULL)
    {
        return NULL;
    }


    for (item = dirAtual->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        if (strcmp(nome, item->diretorio->nomeDir) == 0)
        {
            return item->diretorio;
        }
    }

    printf("\nFINISH SEARCH\n");

    return NULL;
}