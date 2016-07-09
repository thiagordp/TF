//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_FILESYSTEM_H
#define TF_FILESYSTEM_H

#include <stdlib.h>
#include "util.h"
#include "arquivo.h"
#include "diretorio.h"


/**
 * Recebe um comando e um
 * */
void comando(char comando[], size_data_t tamCmd, char dados[], size_data_t tamDados);


long tamanhoFS(diretorio_t *root);

#endif //TF_FILESYSTEM_H
