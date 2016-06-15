//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_DIRETORIO_H
#define TF_DIRETORIO_H

#include "arquivo.h"

#define MAX_SUB_DIR 4
#define MAX_ARQ 4

// Estrutura do arquivo
typedef struct
{
    char *nomeDir;

    long count_arq;
    long count_dir;

    arquivo_t **arquivos;
    diretorio_t **diretorios;
} diretorio_t;


// Cria diretório
void criaDir();

// Apaga Diretório
void apagaDir();

// Abre Diretório
void openDir();

// Fecha Diretório
void closeDir();

#endif //TF_DIRETORIO_H
