//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_DIRETORIO_H
#define TF_DIRETORIO_Hs

#include "arquivo.h"
#include "listaArquivo.h"
#include "listaPasta.h"
#include "util.h"

/**
 * Representação de um diretório/pasta no sistema.
 */
typedef struct
{
    /**
     * Nome do diretório.
     */
    char nomeDir[MAX_SIZE_NAME_ARQ];

    /**
     * Número de arquivos no diretório.
     */
    long count_arq = 0;

    /**
     * Número de subdiretórios.
     */
    long count_dir = 0;

    /**
     * Ponteiro para o primeiro arquivo da pasta.
     */
    itemSubDir_t listasubDirs;

    /**
     *  Ponteiro para o primeiro subdiretório da pasta.
     */
    itemArquivo_t listaArqs;

    /**
     *  Ponteiro para o direito pai do atual.
     */
    diretorio_t *UpDir;

} diretorio_t;

/**
 * Cria diretório
 */
void criaDir(char nome[], diretorio_t &dirAtual);

/**
 * Apaga Diretório
 */
void apagaDir(diretorio_t &dirAtual);

/**
 * Abre Diretório dentro do atual
 */
void openDir(char nome[], diretorio_t &dirAtual);

/**
 * Fecha Diretório
 */
void closeDir(char nome[], diretorio_t &dirAtual);

#endif //TF_DIRETORIO_H
