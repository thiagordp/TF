//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_FILESYSTEM_H
#define TF_FILESYSTEM_H

#include "arquivo.h"
#include "diretorio.h"

diretorio_t *root = NULL;

/**
 * Recebe um comando e um
 * */
void comando(char comando[], size_data_t tamCmd, char dados[], size_data_t tamDados);


// memória compartilhada.
// semáforo

#endif //TF_FILESYSTEM_H
