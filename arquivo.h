//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_ARQUIVO_H
#define TF_ARQUIVO_H

#include "util.h"

typedef struct
{
    char *nomeArq;

    size_data_t tamanho;
    byte *dados;
} arquivo_t;

void criaArq();

void apagaArq();

void abreArq();

void fechaArq();

void leArq();

void escreveArq();


#endif //TF_ARQUIVO_H
