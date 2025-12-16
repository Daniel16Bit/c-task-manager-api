#include "server.h"
#include "http.h"
#include <stdio.h>
#include <ws2tcpip.h>

SOCKET server_init(int porta) {
    WSADATA wsa;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa);
    
    if (result != 0) {
        fprintf(stderr, "Erro ao inicializar Winsock. Codigo: %d\n", result);
        return INVALID_SOCKET;
    }
    
    SOCKET servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET) {
        fprintf(stderr, "Erro ao criar socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }
    
    struct sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(porta);
    endereco.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) == SOCKET_ERROR) {
        fprintf(stderr, "Erro ao fazer bind. Codigo: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return INVALID_SOCKET;
    }
    
    if (listen(servidor, 5) == SOCKET_ERROR) {
        fprintf(stderr, "Erro ao colocar em listen. Codigo: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return INVALID_SOCKET;
    }
    
    printf("Servidor HTTP escutando na porta %d\n", porta);
    printf("Acesse: http://localhost:%d\n\n", porta);
    return servidor;
}

void server_run(SOCKET servidor) {
    char buffer[8192];  // Aumentado para suportar bodies maiores
    
    while (1) {
        printf("Aguardando proximo cliente...\n");
        
        SOCKET cliente = accept(servidor, NULL, NULL);
        if (cliente == INVALID_SOCKET) {
            fprintf(stderr, "Erro ao aceitar conexao. Codigo: %d\n", WSAGetLastError());
            continue;
        }
        
        int bytes_recebidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_recebidos > 0) {
            buffer[bytes_recebidos] = '\0';
            printf("\n=== Requisicao HTTP ===\n");
            
            // Mostrar só a primeira linha (método + caminho)
            char primeira_linha[256];
            sscanf(buffer, "%255[^\r\n]", primeira_linha);
            printf("%s\n", primeira_linha);
            
            // Processar requisição
            http_handle_request(cliente, buffer);
        }
        
        closesocket(cliente);
        printf("Conexao encerrada.\n\n");
    }
}

void server_cleanup(SOCKET servidor) {
    if (servidor != INVALID_SOCKET) {
        closesocket(servidor);
    }
    WSACleanup();
}