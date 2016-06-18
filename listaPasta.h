//
// Created by thiagordp on 16/06/16.
//

#ifndef TF_LISTAPASTA_H
#define TF_LISTAPASTA_H

#include "diretorio.h"

/**
 * Estrutura que contém ponteiros de diretórios.
 */
typedef struct
{
    diretorio_t *diretorio = NULL;

    itemSubDir_t *prevDir = NULL;
    itemSubDir_t *nextDir = NULL;

    // não precisa do pai.
} itemSubDir_t;

#endif //TF_LISTAPASTA_H
