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
    // lista com cabeça
    arquivo_t *arquivo = NULL;
    itemSubDir_t *upDir = NULL;
    itemArquivo_t *prevArq = NULL;
    itemArquivo_t *nextArq = NULL;

} itemArquivo_t;



#endif //TF_LISTAARQUIVO_H
