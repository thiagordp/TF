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
    char extensaoArq[MAX_SIZE_EXT_ARQ];

    /**
     * Tamanho em bytes dos dados do arquivo
     */
    size_data_t tamanho;

    /**
     * Dados do arquivo
     */
    byte *dados;

    /**
     * Diretório que contém o arquivo.
     */
    diretorio_t *UpDir;
} arquivo_t;

/**
 * Cria o arquivo com nome no diretorio atual e retorna o ponteiro para ele.
 */
arquivo_t *criaArq(char nome[], char ext[], diretorio_t *diretorio);

/**
 * Apaga o arquivo e retorna um inteiro indicando o resultado.
 */
byte apagaArq(arquivo_t *arquivo, diretorio_t *diretorio);

/**
 * Abre o arquivo retornando o
 */
arquivo_t *abreArq(char nome[]);

/**
 * Encontra o arquivo com nome indicado e retorna o ponteiro pra ele.
 */
// arquivo_t *procuraArq(char nome[]);

/**
 * Fecha o arquivo com o nome indicado.
 */
byte fechaArq(char *nome);

/**
 * Lê os dados do arquivo indicado.
 */
byte *leArq(arquivo_t *arquivo);

/**
 * Escreve o arquivo com o vetor de bytes no arquivo indicado
 */
byte escreveArq(arquivo_t *arquivo, byte *dados);

#endif //TF_ARQUIVO_H
