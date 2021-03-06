//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_UTIL_H
#define TF_UTIL_H

#include <stdlib.h>
#include <string.h>

#define MAX_SUB_DIR 5
#define MAX_ARQ 5
#define MAX_SIZE_NAME_ARQ 255
#define MAX_SIZE_EXT_ARQ 4
#define byte char
#define size_data_t long
#define MAX_SIZE_ARQ  8388608 //1024 * 1024 * sizeof(byte)
#define MAX_SIZE_BUFFER 1024
#define MAX_SOCKET 10

int tokenize(char *comando, char *delimiters, char ***tokens, size_t *count);

#endif //TF_UTIL_H
