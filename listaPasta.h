//
// Created by thiagordp on 16/06/16.
//

#ifndef TF_LISTAPASTA_H
#define TF_LISTAPASTA_H

#include "def.h"
#include "diretorio.h"

/**
 * Estrutura que contém ponteiros de diretórios.
 */
typedef struct itemSubDir
{
    diretorio_t *diretorio;

    itemSubDir_t *prevDir;
    itemSubDir_t *nextDir;

    // não precisa do pai.
} itemSubDir_t;

#endif //TF_LISTAPASTA_H
