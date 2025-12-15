#ifndef SERVER.H
#define SERVER.H

#include <winsock2.h>

// Iniciando Winsock e criação de sockets na port 
// Retorna: Socket  invalida ou valido (INVALID_SOCKET) em caso de erro
int server_init(int porta);

// Loop principal do server
void server_run(SOCKET servidor);

// Limpa os recursos do servidor (FECHA SOCKET E WINSOCK)
void server_cleanup(SOCKET servidor);


#endif