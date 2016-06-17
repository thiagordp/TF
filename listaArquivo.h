//
// Created by thiagordp on 16/06/16.
//

#ifndef TF_LISTAARQUIVO_H
#define TF_LISTAARQUIVO_H

#include "arquivo.h"

/**
 * Estrutura de ponteiros de arquivos para subpastas
 */
typedef struct
{
    arquivo_t *prevArquivo = NULL;
    arquivo_t *nextArquivo = NULL;
} itemArquivo_t;

#endif //TF_LISTAARQUIVO_H
