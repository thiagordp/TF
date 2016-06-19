//
// Created by thiagordp on 13/06/16.
//

#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include "diretorio.h"


int criaDir(char nome[], diretorio_t *dirAtual)
{
    struct diretorio *novoDiretorio;

    novoDiretorio = (struct diretorio *) malloc(sizeof(struct diretorio));

    strcpy(novoDiretorio->nomeDir, nome);
    novoDiretorio->listaArqs =




    struct itemSubDir *subDir;
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