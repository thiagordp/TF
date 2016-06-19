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
    struct diretorio *diretorio;

    struct itemSubDir *prevDir;
    struct itemSubDir *nextDir;

    // não precisa do pai.
} itemSubDir_t;

#endif //TF_LISTAPASTA_H
