//
// Created by thiagordp on 13/06/16.
//

#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include "diretorio.h"


int criaDir(char nome[], struct diretorio *dirAtual)
{
    struct diretorio *novoDiretorio;

    novoDiretorio = (struct diretorio *) malloc(sizeof(struct diretorio));

    strcpy(novoDiretorio->nomeDir, nome);

    printf("NV -> %s\tAN -> %s\n", novoDiretorio->nomeDir, nome);


    struct itemSubDir *subDir;
}

int apagaDir(char nome[], struct diretorio *dirAtual)
{

}

int listaSubDir(struct diretorio *dirAtual)
{

}

int renomeiaDir(struct diretorio *dirAtual)
{

}