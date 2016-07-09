//
// Created by thiagordp on 18/06/16.
//

#ifndef TF_DEF_H
#define TF_DEF_H

struct diretorio;
struct arquivo;
struct itemArquivo;
struct itemSubDir;

typedef struct diretorio diretorio_t;
typedef struct arquivo arquivo_t;
typedef struct itemArquivo itemArquivo_t;
typedef struct itemSubDir itemSubDir_t;

#define ER_NULL_POINTER     -2
#define ER_EMPTY_STRING     -1
#define ER_NOT_FOUND        -3
#define NO_ERROR             1
#define ER_INVALID_STRING   -4

#endif //TF_DEF_H
