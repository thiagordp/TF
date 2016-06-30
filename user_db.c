#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int seekUser(char* user);
int main(){
	char usuario[30];
	/*
		Abertura do pipe
	*/
	int fd_server, fd_client;
	fd_server = open("pipe.servidor",O_WRONLY);
	fd_client = open("pipe.autenticador",O_RDONLY);		
	if(fd_server < 0){
		printf("Erro ao abrir PIPE servidor\n");
	}
	if(fd_client < 0){
		printf("Erro ao abrir PIPE cliente\n");
	}
	read(fd_client, usuario, sizeof(usuario));
	printf("Usuario recebido do servidor...: %s\n", usuario);
	
	if(seekUser(usuario)==1){
		printf("encontrado\n");
		write(fd_server, "Usuario encontrado", strlen("Usuario encontrado")+1);
	}else{
		write(fd_server, "Usuario nao encontrado", strlen("Usuario nao encontrado")+1);
	}
	close(fd_server);
	close(fd_client);
	return 0;
}

int seekUser(char* user){
	char nome[30];
	FILE *arq;
	arq = fopen("usuarios.dat","r");
	if(!arq){
		printf("Erro de abertura do arquivo \n");
	}
	while(!feof(arq)){
		fscanf(arq, "%s", nome);
		if(strcmp(nome, user) == 0){
			return 1;
		}
	}
	return 0;	
}


