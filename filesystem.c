//
// Created by thiagordp on 13/06/16.
//
#include <string.h>

#include "filesystem.h"
#include "listaPasta.h"
#include "listaArquivo.h"

// Fazer cópia de verdade dos arquivos


void comando(char comando[], size_data_t tamCmd, char dados[], size_data_t tamDados)
{

}

long tamanhoFS(diretorio_t *root)
{
    long tamanho = 0;

    itemSubDir_t *item;
    itemArquivo_t *itemArq;

    for (item = root->listasubDirs->nextDir; item != NULL; item = item->nextDir)
    {
        tamanho += tamanhoFS(item->diretorio);
        tamanho += sizeof(itemSubDir_t) + strlen(item->diretorio->nomeDir);
    }

    for (itemArq = root->listaArqs->nextArq; itemArq != NULL; itemArq = itemArq->nextArq)
    {
        tamanho += sizeof(itemArquivo_t) + itemArq->arquivo->tamanho;
    }

    return tamanho;
}

