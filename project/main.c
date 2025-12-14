/*
===============================================================================================
Este projeto se chama: c-task-manager-api, que é um gerenciador de tarefas em C com terminal como interface (CLI),
persistência em SQLite e backend com API REST criada manualmente com sockets HTTP simples
===============================================================================================
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "database.h"

// Função para extrair o caminho da requisição HTTP
void extrair_requisao(const char *requisicao, char *metodo, char *caminho, size_t tamanho)
{
    char p_linha[256];
    sscanf(requisicao, "%255[^\r\n]", p_linha);

    sscanf(p_linha, "%s %s", metodo, caminho);
}

int main()
{
    // LEMBRETE: Variáveis da Winsock
    WSADATA wsa;
    int result;

    // LEMBRETE: Variáveis do socket e rede
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
    printf("Aguardando conexoes...\n\n");

    // ===== INICIALIZAR BANCO DE DADOS =====
    if (db_init("task.db") != 0)
    {
        printf("Erro ao inicializar banco de dados");
        closesocket(servidor);
        return 1;
    }
    printf("Banco de dados inicializado!\n\n");

    while (1)
    {
        printf("Aguardando proximo cliente...\n");
        cliente = accept(servidor, NULL, NULL);
        if (cliente == INVALID_SOCKET)
        {
            printf("Erro ao aceitar conexao. Codigo: %d\n", WSAGetLastError());
            continue;
        }

        bytes_recebidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);

        if (bytes_recebidos > 0)
        {
            buffer[bytes_recebidos] = '\0';
            printf("\n=== Requisicao recebida ===\n%s\n", buffer);

            // Extrai o caminho da requisição
            char metodo[10];
            char caminho[256];
            extrair_requisao(buffer, metodo, caminho, sizeof(caminho));
            printf(">>> Metodo: %s | Caminho: %s\n\n", metodo, caminho);

            const char *resposta;

            // ===== ROTA: GET / =====
            if (strcmp(metodo, "GET") == 0 && strcmp(caminho, "/") == 0)
            {
                resposta =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "\r\n"
                    "<h1>API Task Manager</h1>"
                    "<p>Rotas disponiveis:</p>"
                    "<ul>"
                    "<li>GET /tasks - Lista todas as tarefas</li>"
                    "<li>GET /tasks/:id - Obtem uma tarefa especifica</li>"
                    "<li>POST /tasks - Cria uma nova tarefa</li>"
                    "<li>PUT /tasks/:id - Atualiza uma tarefa</li>"
                    "<li>DELETE /tasks/:id - Deleta uma tarefa</li>"
                    "</ul>";
            }
            // ===== ROTA: GET /tasks =====
            else if (strcmp(metodo, "GET") == 0 && strcmp(caminho, "/tasks") == 0)
            {
                int count;
                Task *tasks = db_get_all_tasks(&count);

                if (count == 0)
                {
                    resposta =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"tasks\": []}";
                }
                else
                {
                    // Montar JSON manualmente
                    char json_body[4086] = "{\"tasks\": [";

                    for (int i = 0; i < count; i++)
                    {
                        char task_json[512];
                        snprintf(task_json, sizeof(task_json),
                                 "{\"id\": %d, \"title\": \"%s\", \"completed\": %d}%s",
                                 tasks[i].id, tasks[i].title, tasks[i].completed,
                                 (i < count - 1) ? "," : "");
                        strcat(json_body, task_json);
                    }
                    strcat(json_body, "]}");

                    char http_response[5000];
                    snprintf(http_response, sizeof(http_response),
                             "HTTP/1.1 200 OK\r\n"
                             "Content-Type: application/json\r\n"
                             "\r\n"
                             "%s",
                             json_body);

                    send(cliente, http_response, strlen(http_response), 0);
                    free(tasks);

                    closesocket(cliente);
                    printf("Resposta enviada ao cliente!\n");
                    printf("Conexao encerrada.\n\n");
                    continue;
                }
            }
            // ===== ROTA: GET /tasks/:id =====
            else if (strcmp(metodo, "GET") == 0 && strncmp(caminho, "/tasks/", 7) == 0){
                int task_id = atoi(caminho + 7);

            Task * task = db_get_task_by_id(task_id);

                char json_resposta[1024];
                if(task){
                    snprintf(json_resposta, sizeof(json_resposta),
                                "HTTP/1.1 200 OK\r\n"
                                "Content-Type: application/json\r\n"
                                "\r\n"
                                "{\"id\": %d, \"title\": \"%s\", \"completed\": %d}",
                                task->id, task-> title, task->completed);
                    free(task);
                }else{
                    snprintf(json_resposta, sizeof(json_resposta),
                            "HTTP/1.1 404 Not Found\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"error\": \"Tarefa nao encontrada\"}");
                }

                send(cliente, json_resposta, strlen(json_resposta), 0);
                printf("Resposta enviada ao cliente!\n");

                closesocket(cliente);
                printf("Conexao encerrada.\n\n");
                continue;
            }

            // ===== ROTA: POST /tasks =====
            else if (strcmp(metodo, "POST") == 0 && strcmp(caminho, "/tasks") == 0){
                // Por enquanto, cria com titulo padrão
                // TODO: Parsear JSON do body depois

                 int novo_id = db_create_task("Nova tarefa (via POST)", 0);

                
                 char json_resposta[512];
                 if(novo_id > 0){
                    snprintf(json_resposta, sizeof(json_resposta),
                             "HTTP/1.1 201 Created\r\n"
                             "Content-Type: application/json\r\n"
                             "\r\n"
                             "{\"message\": \"Tarefa criada\", \"id\": %d}", novo_id);         
                 }else{
                    snprintf(json_resposta, sizeof(json_resposta),
                             "HTTP/1.1 500 Internal Server Error\r\n"
                             "Content-Type: application/json\r\n"
                             "\r\n"
                             "{\"error\": \"Erro ao criar tarefa\"}");
                 }
            
                send(cliente, json_resposta, strlen(json_resposta), 0);
                printf("Resposta enviada ao cliente\n");

                closesocket(cliente);
                printf("Conexao encerrada.\n\n");
                continue;

            }


            // ===== ROTA: PUT /tasks/:id =====
            else if (strcmp(metodo, "PUT") == 0 && strncmp(caminho, "/tasks/", 7) == 0)
            {
                int task_id = atoi(caminho + 7);
                // Por enquanto, atualiza com titulo padrão
                // TODO: Parsear JSON do body depois
                int resultado = db_update_task(task_id, "Tarefa atualizada(VIA PUT)", 1);

                char json_resposta[512];
                if(resultado == 0){
                   snprintf(json_resposta, sizeof(json_resposta),
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"message\": \"Tarefa %d atualizada\"}", task_id);
                }else{
                    snprintf(json_resposta, sizeof(json_resposta),
                            "HTTP/1.1 500 Internal Server Error\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"error\": \"Erro ao atualizar tarefa\"}");
                }

                send(cliente, json_resposta, strlen(json_resposta), 0);
                printf("Resposta enviada ao cliente\n");

                closesocket(cliente);
                printf("Conexao encerrada.\n\n");
                continue;
            }


            // ===== ROTA: DELETE /tasks/:id =====
            else if (strcmp(metodo, "DELETE") == 0 && strncmp(caminho, "/tasks/", 7) == 0)
            {
                int task_id = atoi(caminho + 7);

                int resultado = db_delete_task(task_id);

                char json_resposta[512];
                if(resultado == 0){ 
                snprintf(json_resposta, sizeof(json_resposta),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"message\": \"Tarefa %d deletada\"}", task_id);
                } else {
                   snprintf(json_resposta, sizeof(json_resposta),
                           "HTTP/1.1 500 Internal Server Error\r\n"
                           "Content-Type: application/json\r\n"
                           "\r\n"
                           "{\"error\": \"Erro ao deletar tarefa\"}");
                }
                send(cliente, json_resposta, strlen(json_resposta), 0);
                printf("Resposta enviada ao cliente!\n");

                closesocket(cliente);
                printf("Conexao encerrada.\n\n");
                continue;
            }
            // ===== ROTA NÃO ENCONTRADA =====
            else
            {
                resposta =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\": \"Rota nao encontrada\"}";
            }

            send(cliente, resposta, strlen(resposta), 0);
            printf("Resposta enviada ao cliente!\n");
        }

        closesocket(cliente);
        printf("Conexao encerrada.\n\n");
    }
    db_close();
    WSACleanup();
    return 0;
}