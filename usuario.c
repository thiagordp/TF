#include "usuario.h"
#include<stdio.h>

//
// Created by thiagordp on 10/07/16.
//
usuario_t *criaUsuario(char nome[], diretorio_t *dirAtual)
{
    // Decl. do ponteiro pro novo diretório
    usuario_t *novoUsuario = NULL;

    // Aloca dinamicamente
    novoUsuario = (usuario_t *) malloc(sizeof(usuario_t));
    novoUsuario->nome = (char *) calloc(strlen(nome + 1), sizeof(char));

    novoUsuario->dirAtual = NULL;

    // Copia o nome para a estrutura do diretório.
    strcpy(novoUsuario->nome, nome);

    // Avança para o diretório indicado
    avancaDir(novoUsuario, dirAtual);

    return novoUsuario;
}

int avancaDir(usuario_t *user, diretorio_t *nextDir)
{
    itemSubDir_t *item = NULL;

    // Verificação de erros
    if (user == NULL || nextDir == NULL)
    {
        return ER_NULL_POINTER;
    }

    // Alocação do novo diretório.
    item = (itemSubDir_t *) malloc(sizeof(itemSubDir_t));
    item->diretorio = nextDir;

    /*
     * Ajustes do ponteiros
     */

    // Caso não exista nenhum diretório atual
    if (user->dirAtual == NULL)
    {
        item->prevDir = NULL;
        item->nextDir = NULL;

        user->dirAtual = item;
    }
    else // Caso já exista um diretório atual.
    {
        item->prevDir = NULL;
        item->nextDir = user->dirAtual;
        item->nextDir->prevDir = item;

        user->dirAtual = item;
    }

    return NO_ERROR;
}

int retrocedeDir(usuario_t *user)
{
    itemSubDir_t *item = NULL;

    // Verificação de parâmetros corretos.
    if (user == NULL)
    {
        return ER_NULL_POINTER;
    }
    if (user->dirAtual == NULL)
    {
        return NO_ERROR;
    }

    item = user->dirAtual;              // Variável auxiliar.

    if (item->nextDir != NULL)
    {
        item->nextDir->prevDir = NULL;  // O diretório a seguir aponta para NULL;
    }
    else // caso tenha só um item.
    {
        return NO_ERROR;
    }

    user->dirAtual = item->nextDir;     // Pode ser NULL ou não


    free(item);

    return NO_ERROR;
}

int printCaminho(usuario_t *user)
{
    if (user == NULL)
    {
        return ER_NULL_POINTER;
    }

    itemSubDir_t *item;
    for (item = user->dirAtual; item != NULL && item->nextDir != NULL; item = item->nextDir);

    if (item == NULL)
    {
        printf("\n%s@%s:~$", user->nome, user->nome);
        return (NO_ERROR);
    }

    printf("\n%s@%s:", user->nome, user->nome);

    for (; item != NULL; item = item->prevDir)
    {
        printf("/%s", item->diretorio->nomeDir);
    }
    printf("$ ");

    return NO_ERROR;
}