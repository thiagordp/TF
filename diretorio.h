//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_DIRETORIO_H
#define TF_DIRETORIO_Hs

#include "def.h"
#include "arquivo.h"

/*
#include "listaArquivo.h"
#include "listaPasta.h"*/

#include "util.h"


/**
 * Representação de um diretório/pasta no sistema.
 */
typedef struct diretorio
{
    /**
     * Nome do diretório.
     */
    char nomeDir[MAX_SIZE_NAME_ARQ];

    /**
     * Número de arquivos no diretório.
     */
    long count_arq;

    /**
     * Número de subdiretórios.
     */
    long count_dir;

    /**
     * Ponteiro para o primeiro arquivo da pasta.
     */
    struct itemSubDir *listasubDirs;

    /**
     *  Ponteiro para o primeiro subdiretório da pasta.
     */
    struct itemArquivo *listaArqs;
} diretorio_t;

/**
 * Cria diretório
 */
int criaDir(char nome[], struct diretorio *dirAtual);

/**
 * Apaga Diretório
 */
int apagaDir(char nome[], struct diretorio *dirAtual);

/*Listagem dos subdiretórios*/
int listaSubDir(struct diretorio *dirAtual);

/*Renomear o nome do diretório.*/
int renomeiaDir(struct diretorio *dirAtual);

#endif //TF_DIRETORIO_H
