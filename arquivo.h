//
// Created by thiagordp on 13/06/16.
//
#ifndef TF_ARQUIVO_H
#define TF_ARQUIVO_H

#include "util.h"
#include "def.h"

/**
 * Estrutura que representa um arquivo do sistema.
 */
typedef struct arquivo
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
struct arquivo *criaArq(char nome[], char dados[], struct diretorio *dirAtual);

/**
 * Apaga o arquivo e retorna um inteiro indicando o resultado.
 */
byte apagaArq(char nome[], struct diretorio *dirAtual);

/**
 * Abre o arquivo retornando o
 */
struct arquivo *abreArq(char nome[], struct diretorio *dirAtual);

/**
 * Encontra o arquivo com nome indicado e retorna o ponteiro pra ele.
 */
// arquivo_t *procuraArq(char nome[]);

/**
 * Lê os dados do arquivo indicado.
 */
byte *leArq(char nome[], struct diretorio *dirAtual);

/**
 * Escreve o arquivo com o vetor de bytes no arquivo indicado
 */
byte escreveArq(struct arquivo *arquivo, byte *dados);

#endif //TF_ARQUIVO_H
