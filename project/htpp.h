#ifndef HTTP_H
#define HTTP_H

#include <winsock2.h>

//Processa Requisição HTTP e envia resposta apropriada
void http_handle_request(SOCKET cliente, const char *requisicao);

#endif