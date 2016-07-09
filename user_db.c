#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int seekUser(char *user);

int main()
{
    char usuario[30];
    /*
        Abertura do pipe
    */
    int fd_server, fd_client;

    while (1)
    {
        printf("USER_DB: Abrindo pipe...\n");

        fd_server = open("pipe.servidor", O_WRONLY);
        fd_client = open("pipe.autenticador", O_RDONLY);

        if (fd_server >= 0 && fd_client >= 0)
        {
            printf("USER_DB: Lendo do servidor...\n");

            bzero(usuario, sizeof(usuario));

            while (strlen(usuario) == 0)
            {
                read(fd_client, usuario, sizeof(usuario));
            }

            printf("USER_DB: Usuario recebido do servidor...: %s\n", usuario);

            if (seekUser(usuario) == 1)
            {
                printf("USER_DB: encontrado\n");
                write(fd_server, "V", strlen("V") + 1);
            } else
            {
                printf("USER_DB: nao encontrado\n");
                write(fd_server, "I", strlen("I") + 1);
            }

            close(fd_server);
            close(fd_client);
        }
        else
        {
            printf("USER_DB: erro ao abrir pipe com o servidor\n");
        }
    }
}

int seekUser(char *user)
{
    char nome[30];
    FILE *arq;
    arq = fopen("usuarios.dat", "r");
    if (!arq)
    {
        printf("Erro de abertura do arquivo \n");
    }
    while (!feof(arq))
    {
        fscanf(arq, "%s", nome);
        if (strcmp(nome, user) == 0)
        {
            return 1;
        }
    }
    return 0;
}


