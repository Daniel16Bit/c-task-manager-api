
/*
Este projeto se chama: c-task-manager-api, que é um gerenciador de tarefas em C com terminal como interface (CLI),
persistência em SQLite e backend com API REST criada manualmente com sockets HTTP simples
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

int main()
{

    // LEMBRETE: Variáveis da Winsock
    WSADATA wsa;
    int result;

    // LEMBBRETE: Variáveis do socket e rede
    SOCKET servidor; // ← Socket "mãe" que ESCUTA conexões
    SOCKET cliente;  // ← Socket "filho" que CONVERSA com UM cliente específico
    struct sockaddr_in endereco;
    int resultado_bind;
    int resultado_listen;
    char buffer[4096];
    int bytes_recebidos;

    result = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (result != 0)
    {
        printf("Erro ao inicializar o Winsock. Codigo: %d\n", result);
        return 1;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET)
    {
        printf("Erro ao inicializar o Socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    endereco.sin_family = AF_INET;         // Protocolo IPV4
    endereco.sin_port = htons(8080);       // Converte a porta 8080 para FORMATO de rede (big-endian)
    endereco.sin_addr.s_addr = INADDR_ANY; // Aceita que qlq IP faça conexão no servidor

    resultado_bind = bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco));
    if (resultado_bind == SOCKET_ERROR)
    {
        printf("Erro ao fazer o Bind. Codigo de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    resultado_listen = listen(servidor, 5);
    if (resultado_listen == SOCKET_ERROR)
    {
        printf("Erro ao colocar socket em modo listen. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket Criado com sucesso!\n");
    printf("Servidor escutando na porta 8080...\n");
    printf("Aguardando conexão de usuario...\n");

    cliente = accept(servidor, NULL, NULL);
    if (cliente == INVALID_SOCKET)
    {
        printf("Erro ao aceitar conexao. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    bytes_recebidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);

    if (bytes_recebidos > 0)
    {
        buffer[bytes_recebidos] = '\0';
        printf("\n=== Requisicao recebida ===\n%s\n", buffer);

        const char *resposta =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 68\r\n"
            "\r\n"
            "<h1>Servidor C Funcionando!</h1><p>c-task-manager-api v0.1</p>";

        send(cliente, resposta, strlen(resposta), 0);
        printf("Resposta enviada ao cliente\n");
    }

    closesocket(cliente);
    printf("Conexao encerrada.\n\n");

    printf("Servidor continua rodando...\n");
    printf("Pressione Ctrl+C para encerrar.\n");

    // Loop infinito para aceitar múltiplos clientes
    
    while (1)
    {
        Sleep(1000); // Aguarda 1 segundo
    }
    return 0;
}