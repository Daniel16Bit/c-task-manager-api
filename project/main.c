#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>


int main (){

    WSADATA wsa;
    int result;
    SOCKET servidor;

    result = WSAStartup(MAKEWORD(2,2), &wsa);

    if(result != 0){
        printf("Erro ao inicializar o Winsock. Codigo: %d\n", result);
        return 1;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if(servidor == INVALID_SOCKET){
        printf("Erro ao inicializar o Socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket Criado com sucesso!\n");


    
    WSACleanup();
    return 0;
}