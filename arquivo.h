//
// Created by thiagordp on 13/06/16.
//
#ifndef TF_ARQUIVO_H
#define TF_ARQUIVO_H

#include "util.h"
#include "diretorio.h"

/**
 * Estrutura que representa um arquivo do sistema.
 */
typedef struct
{
    /**
     * Nome do arquivo
     */
    char nomeArq[MAX_SIZE_NAME_ARQ];

    /**
     * Extensão do arquivo
     */
    // char extensaoArq[MAX_SIZE_EXT_ARQ];

    /**
     * Tamanho em bytes dos dados do arquivo
     */
    size_data_t tamanho;

    /**
     * Dados do arquivo
     */
    byte *dados;

} arquivo_t;

/**
 * Cria o arquivo com nome no diretorio atual e retorna o ponteiro para ele.
 */
arquivo_t *criaArq(char nome[], char dados[], diretorio_t *diretorio);

/**
 * Apaga o arquivo e retorna um inteiro indicando o resultado.
 */
byte apagaArq(char nome[], diretorio_t *diretorio);

/**
 * Abre o arquivo retornando o
 */
arquivo_t *abreArq(char nome[], diretorio_t* diretorio);

/**
 * Encontra o arquivo com nome indicado e retorna o ponteiro pra ele.
 */
// arquivo_t *procuraArq(char nome[]);

/**
 * Lê os dados do arquivo indicado.
 */
byte *leArq(char nome[], diretorio_t* diretorio);

/**
 * Escreve o arquivo com o vetor de bytes no arquivo indicado
 */
byte escreveArq(arquivo_t *arquivo, byte *dados);

#endif //TF_ARQUIVO_H
