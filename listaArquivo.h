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
    arquivo_t *arquivo;
    itemArquivo_t *prevArq;
    itemArquivo_t *nextArq;

} itemArquivo_t;


#endif //TF_LISTAARQUIVO_H
