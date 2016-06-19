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
    itemSubDir_t *listasubDirs;

    /**
     *  Ponteiro para o primeiro subdiretório da pasta.
     */
    itemArquivo_t *listaArqs;
} diretorio_t;

/**
 * Cria diretório
 */
int criaDir(char nome[], diretorio_t *dirAtual);

/**
 * Apaga Diretório
 */
int apagaDir(char nome[], diretorio_t *dirAtual);

/*Listagem dos subdiretórios*/
int listaSubDir(diretorio_t *dirAtual);

/*Renomear o nome do diretório.*/
int renomeiaDir(diretorio_t *dirAtual);

#endif //TF_DIRETORIO_H
