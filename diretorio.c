//
// Created by thiagordp on 13/06/16.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "diretorio.h"
#include "listaArquivo.h"
#include "listaPasta.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

diretorio_t *criaDir(char nome[], diretorio_t **dirAtual)
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
    }


    return novoDiretorio;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int apagaDir(char nome[], diretorio_t *dirAtual)
{
    if (dirAtual == NULL)
    {
        return ER_NULL_POINTER;
    }

    // se o nome for vazio, apaga tudo o que tem dentro do diretório.
    if (strlen(nome) == 0)
    {
        //if (dirAtual->count_arq > 0) // REVISAR: talvez tenha que passar de qualquer forma
        //{

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
        diretorio_t *dirAt;
        itemSubDir_t *item;

        for (item = dirAtual->listasubDirs->nextDir;
             item != NULL && item->diretorio != NULL &&
             strcmp(item->diretorio->nomeDir, nome) != 0;
             item = item->nextDir);

        if (item == NULL || item->diretorio == NULL)
        {
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

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *listaSubDir(diretorio_t *dirAtual)
{
    char *imprimirLista = NULL;
    imprimirLista = (char *) malloc(sizeof(char) * 256);
    strcpy(imprimirLista, "Diretorios: \n");
    /*printf("---------------------------------\n> %s\n---------------------------------\n",
           dirAtual->nomeDir);
    printf("Nome\t\t\tEnd\t\t\tNo Arqs\t\tNo Dirs\t\t\n");*/

    itemSubDir_t *item;
    diretorio_t *dir;

    for (item = dirAtual->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        dir = item->diretorio;
        strcat(imprimirLista, dir->nomeDir);
        strcat(imprimirLista, "\n");
        printf("%s\t\t\t%p\t\t\t%ld\t\t%ld\n", dir->nomeDir, dir, dir->count_arq, dir->count_dir);
    }
    //printf("%s\n",imprimirLista);
    return imprimirLista;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *listaArquivo(diretorio_t *dirAtual)
{

    char *imprimirLista = NULL;
    imprimirLista = (char *) malloc(sizeof(char) * 256);
    strcpy(imprimirLista, "Arquivos: \n");

    /*printf("---------------------------------\n> %s\n---------------------------------\n",
           dirAtual->nomeDir);
    printf("Nome\t\t\tEnd\t\t\tNo Arqs\t\tNo Dirs\t\t\n");*/

    itemArquivo_t *item;
    arquivo_t *arq;

    for (item = dirAtual->listaArqs->nextArq; item != NULL; item = item->nextArq)
    {
        arq = item->arquivo;
        strcat(imprimirLista, arq->nomeArq);
        strcat(imprimirLista, "\n");
        //printf("%s\t\t\t%p\t\t\t%ld\n", arq->nomeArq, arq, arq->tamanho);
    }
    printf("%s\n", imprimirLista);
    return imprimirLista;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int renomeiaDir(char nome[], diretorio_t *dirAtual)
{
    if (dirAtual == NULL)
    {
        return ER_NULL_POINTER;
    }
    else if (strlen(nome) == 0)
    {
        return ER_EMPTY_STRING;
    }
    else if (strlen(nome) > MAX_SIZE_NAME_ARQ)
    { // Corrigir o tamnho
        return 0;
    }

    strcpy(dirAtual->nomeDir, nome);

    return NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

diretorio_t *procuraDiretorio(char *nome, diretorio_t *dirAtual)
{
    itemSubDir_t *item;

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

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

diretorio_t *procuraDiretorioRec(char **tokens, int n, diretorio_t *dirAtual)
{
    diretorio_t *dir = NULL;

    for (int i = 0; i < n; i++)
    {
        dir = procuraDiretorio(tokens[i], dirAtual);

        if (dir == NULL)
        {
            return NULL;
        }

        dirAtual = dir;
    }

    return dir;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int moveDir(char nome[], diretorio_t *dirOrigem, diretorio_t *dirDestino)
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

    // Prosseguimento sem erros de parâmetros

    itemSubDir_t *item;

    for (item = dirOrigem->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        if (strcmp(nome, item->diretorio->nomeDir) == 0)
        {
            break;
        }
    }

    // Caso não exista na lista retorna NULL
    if (item == NULL)
    {
        return ER_NOT_FOUND;
    }

    // Arranjo dos ponteiros do diretório original.
    item->prevDir->nextDir = item->nextDir;
    if (item->nextDir != NULL)
    {
        item->nextDir->prevDir = item->prevDir;
    }

    // Arranjo dos ponteiros do diretório a ser movido.
    item->prevDir = dirDestino->listasubDirs;
    item->nextDir = dirDestino->listasubDirs->nextDir;

    // Arranjo dos ponteiros do diretório de destino.
    dirDestino->listasubDirs->nextDir = item;
    if (item->nextDir != NULL)
    {
        item->nextDir->prevDir = item;
    }

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copiaInfoDir(diretorio_t *dirOrigem, diretorio_t **dirDestino)
{
    if (*dirDestino != NULL)
    {
        free(dirDestino);
    }

    *dirDestino = (diretorio_t *) malloc(sizeof(diretorio_t));

    // Cópia do nome da pasta.
    // strcpy((*dirDestino)->nomeDir, dirOrigem->nomeDir);

    criaDir(dirOrigem->nomeDir, dirDestino);

    // Cópia do tamanho dos diretórios.
    (*dirDestino)->count_arq = dirOrigem->count_arq;
    (*dirDestino)->count_dir = dirOrigem->count_dir;

    itemSubDir_t *item;
    for (item = dirOrigem->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        itemSubDir_t *novoItem = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));

        diretorio_t *dir = NULL;
        copiaDir("", item->diretorio, dir);
    }

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copiaDir(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino)
{
    // ####################   Verificações iniciais   ################### //
    if (strlen(nomeDir) == 0)
    {
        return ER_EMPTY_STRING;
    }
    else if (dirDestino == NULL || dirOrigem == NULL)
    {
        return ER_NULL_POINTER;
    }
    // ######################## Lógica dos esquemas ###################### //
    diretorio_t *destino = NULL;
    diretorio_t *dir = procuraDiretorio(nomeDir, dirOrigem);

    if (dir == NULL)
    {
        return ER_NOT_FOUND;
    }

    return NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copyDir(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino)
{
    diretorio_t *dir, *dirCopia;

    // ####################   Verificações iniciais   ################### //
    if (dirDestino == NULL || dirOrigem == NULL)
    {
        return ER_NULL_POINTER;
    }

    // ######################## Lógica dos esquemas ###################### //

    // Caso seja vazio, o diretório origem em si vai ser copiado para a pasta destino
    if (strlen(nomeDir) == 0)
    {
        dir = dirOrigem;
    }
        // Se não, copia o arquivo com o nome indicado dentro da pasta origem.
    else
    {
        // procura pelo diretório que será copiado
        dir = procuraDiretorio(nomeDir, dirOrigem);


        // caso esse diretório não exista.
        if (dir == NULL)
        {
            return ER_NOT_FOUND;
        }
    }

    // Cria um novo diretório que será a cópia na pasta de destino.
    dirCopia = criaDir(dir->nomeDir, &dirDestino);

    if (dirCopia == NULL)
    {
        return ER_NULL_POINTER;
    }

    for (itemSubDir_t *item = dir->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        copyDir("", item->diretorio, dirCopia);
    }

    for (itemArquivo_t *item = dir->listaArqs->nextArq; item != NULL; item = item->nextArq)
    {
        criaArq(item->arquivo->nomeArq, item->arquivo->dados, dirCopia);
    }

    return NO_ERROR;
}
