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
    diretorio_t *prevDiretorio = NULL;
    diretorio_t *nextDiretorio = NULL;
} itemSubDir_t;


#endif //TF_LISTAPASTA_H
