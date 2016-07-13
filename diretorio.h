//
// Created by thiagordp on 13/06/16.
//

#ifndef TF_DIRETORIO_H
#define TF_DIRETORIO_H

#include "def.h"
#include "arquivo.h"
#include "util.h"

/**
 * Representação de um diretório/pasta no sistema.
 */
typedef struct diretorio
{
    /**
     * Nome do diretório.
     */
    char nomeDir[MAX_SIZE_NAME_ARQ];

    /**
     * Número de arquivos no diretório.
     */
    long count_arq;

    /**
     * Número de subdiretórios.
     */
    long count_dir;

    /**
     * Ponteiro para o primeiro arquivo da pasta.
     */
    itemSubDir_t *listasubDirs;

    /**
     *  Ponteiro para o primeiro subdiretório da pasta.
     */
    itemArquivo_t *listaArqs;
} diretorio_t;

/**
 * Cria diretório
 */
diretorio_t *criaDir(char nome[], diretorio_t **dirAtual);

/**
 * Apaga o diretório com o nome indicado que está dentro da pasta indicada.
 */
int apagaDir(char nome[], diretorio_t *dirAtual);

/**
 * Listagem dos subdiretórios
 */
char *listaSubDir(diretorio_t *dirAtual);

/**
 * Lista os arquivos do diretório indicado.
 */
char *listaArquivo(diretorio_t *dirAtual);

/*
 * Renomear o nome do diretório.
 */
int renomeiaDir(char nome[], diretorio_t *dirAtual);

/**
 *
 */
diretorio_t *procuraDiretorio(char nome[], diretorio_t *dirAtual);

/**
 * Retorna o ponteiro da pasta indicado no caminho
 * */
diretorio_t *procuraDiretorioRec(char **tokens, int n, diretorio_t *dirAtual);

/**
 * Move o diretório que outro
 *
 * @param nome        Nome do arquivo que será movido.
 * @param dirOrigem   Diretório de origem
 * @param dirDestino  Diretório de destino
 *
 * @return Status da operação
 * */
int moveDir(char nome[], diretorio_t *dirOrigem, diretorio_t *dirDestino);

/**
 * Varre o sistema de arquivos em busca de um arquivo com o nome indicado,
 * a partir do diretório raiz também indicado.
 *
 * @param nome | Nome do arquivo
 * @param root | Diretório raiz por onde a busca começará
 *
 * @return Status da operação
 */
// arquivo_t *varreArquivo(char nome[], diretorio_t *root);

int copiaInfoDir(diretorio_t *dirOrigem, diretorio_t **dirDestino);

int copiaDir(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino);

/**
 * Cópia de todo o conteúdo de um diretório de nome indicado na pasta de
 * origem para outro diretório destino.
 * */
int copyDir(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino);

#endif //TF_DIRETORIO_H
