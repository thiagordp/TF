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
void ListarArquivos(char *comando);

void ListarConteudo(char *comando);

void CriarDiretorio(char *comando);

void CriarArquivo(char *comando);

void mostraConteudoArquivo(char *comando);

void Remover(char *comando);

void Mover(char *comando);

void Copiar(char *comando);

void Entrar(char *comando);

void ComandoErrado();

long tamanhoFS(diretorio_t *root);

#endif //TF_FILESYSTEM_H
