//
// Created by thiagordp on 10/07/16.
//

#include "util.h"
#include "def.h"


int tokenize(char *comando, char *delimiters, char ***tokens, size_t *count)
{
    char *token = NULL;
    size_t cnt = 0;

    if (strlen(comando) == 0 || strlen(delimiters) == 0) return ER_EMPTY_STRING;
    if (*tokens != NULL) free(*tokens);
    if (count == NULL) return ER_NULL_POINTER;

    *tokens = NULL;

    token = strtok(comando, delimiters);

    while (token != NULL)
    {
        cnt++;
        *tokens = (char **) realloc(*tokens, cnt * sizeof(char *));
        (*tokens)[cnt - 1] = token;
        //printCaminho(user);
        token = strtok(NULL, delimiters);
    }

    *count = cnt;

    return NO_ERROR;
}
