//
// Created by thiagordp on 10/07/16.
//

#ifndef TF_USUARIO_H
#define TF_USUARIO_H

#include "diretorio.h"
#include "util.h"
#include "listaPasta.h"


typedef struct usuario
{
    char *nome;
    itemSubDir_t *dirAtual;
} usuario_t;

usuario_t *criaUsuario(char nome[], diretorio_t *dirAtual);

/**
 * Avanca para o diretório indicado
 * */
int avancaDir(usuario_t *user, diretorio_t *nextDir);

/*
 * Retrocede uma pasta
 * */
int retrocedeDir(usuario_t *user);

/**
 * Imprime o caminho de diretório do user.
 * */
char* printCaminho(usuario_t *user);

#endif //TF_USUARIO_H
