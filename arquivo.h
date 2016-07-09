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
    char dados[MAX_SIZE_ARQ];

} arquivo_t;

/**
 * Cria o arquivo com nome no diretorio atual que deve existir obrigatoriamente e
 * retorna o ponteiro para ele.
 */
arquivo_t *criaArq(char nome[], byte dados[], diretorio_t *diretorio);

/**
 * Apaga o arquivo e retorna um inteiro indicando o resultado.
 */
short apagaArq(char nome[], diretorio_t *dirAtual);

/**
 * Abre o arquivo retornando o ponteiro para este a partir do nome do arquivo e do diretório atual.
 */
arquivo_t *abreArq(char nome[], diretorio_t *dirAtual, itemArquivo_t **item);

/**
 * Encontra o arquivo com nome indicado e retorna o ponteiro pra ele.
 */
// arquivo_t *procuraArq(char nome[]);

/**
 * Lê os dados do arquivo indicado.
 */
byte *leArq(char nome[], diretorio_t *diretorio);

/**
 * Escreve o arquivo com o vetor de bytes no arquivo indicado
 *
 * @return Retorna o status da operação.
 *  -2 - Ponteiro Nulo
 *  -1 - ...
 *   0 - Quantidade de dados além do permitido.
 *   1 - OK
 */
short escreveArq(arquivo_t *arq, byte dados[]);

/**
 * Renomeia o arquivo apontado com o nome indicado.
 *
 * @return Indicação de operação concluída ou não.
 *  -2 - Ponteiro nulo.
 *  -1 - Nome vazio.
 *   0 - muito grande.
 *   1 - OK.
 */
short renomeiaArq(char nome[], arquivo_t *arquivo);

/**
 * Calcula o tamanho do arquivo somando o números de bytes do nome e dos dados.
 */
size_data_t sizeArq(arquivo_t *arq);

/**
 * Move o diretório que outro
 *
 * @param nome        Nome do arquivo que será movido.
 * @param dirOrigem   Diretório de origem
 * @param dirDestino  Diretório de destino
 *
 * @return Status da operação
 * */
int moveArquivo(char nome[], diretorio_t *dirOrigem, diretorio_t *dirDestino);

/**
   Copia o conteúdo total de um arquivo para outro.
   Ponteiros para os arquivos de origem e destino

   @Return Estado da operação.
 */
int copiaInfoArquivo(arquivo_t *arqOrigem, arquivo_t **arqDestino);

int copiaArquivo(char nomeDir[], diretorio_t *dirOrigem, diretorio_t *dirDestino);

/**
 * Varre o sistema de diretório em busca de um arquivo com
 * 3o nome indicado a partir do diretório atual.
 */
//
//
//int varreArquivo(char nome[], diretorio_t *root, arquivo_t **arq)


/**
 * Procura o arquivo com o nome na pasta indicada.
 * */
arquivo_t *procuraArq(char nomeArq[], diretorio_t *dirAtual);

#endif //TF_ARQUIVO_H
