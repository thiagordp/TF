//
// Created by thiagordp on 16/06/16.
//

#ifndef TF_LISTAARQUIVO_H
#define TF_LISTAARQUIVO_H

#include "def.h"
#include "arquivo.h"

/**
 * Estrutura de ponteiros de arquivos para subpastas
 */
typedef struct itemArquivo
{
    // lista com cabeça
    struct arquivo *arquivo;
    struct itemSubDir *upDir;
    struct itemArquivo *prevArq;
    struct itemArquivo *nextArq;

} itemArquivo_t;


#endif //TF_LISTAARQUIVO_H
