#include "http.h"
#include "database.h"
#include "json_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Parser: Extrai método e caminho da requisição
static void parse_request(const char *requisicao, char *metodo, char *caminho)
{
    char primeira_linha[256];
    sscanf(requisicao, "%255[^\r\n]", primeira_linha);
    sscanf(primeira_linha, "%s %s", metodo, caminho);
}

// Parser: Extrai o body (depois de \r\n\r\n)
static const char *extract_body(const char *requisicao)
{
    const char *body_start = strstr(requisicao, "\r\n\r\n");
    if (body_start)
    {
        return body_start + 4;
    }
    return "";
}

static void send_response(SOCKET cliente, const char *resposta)
{
    send(cliente, resposta, strlen(resposta), 0);
}

// Rota: GET /
static void route_home(SOCKET cliente)
{
    const char *resposta =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "\r\n"
        "<html><head><meta charset='utf-8'></head><body>"
        "<h1>🗂️ C Task Manager API</h1>"
        "<p>API REST para gerenciamento de tarefas</p>"
        "<h2>Rotas Disponíveis:</h2>"
        "<ul>"
        "<li><strong>GET /tasks</strong> - Lista todas as tarefas</li>"
        "<li><strong>GET /tasks/:id</strong> - Obtém uma tarefa específica</li>"
        "<li><strong>POST /tasks</strong> - Cria nova tarefa (Body: {\"title\": \"...\", \"completed\": 0})</li>"
        "<li><strong>PUT /tasks/:id</strong> - Atualiza tarefa (Body: {\"title\": \"...\", \"completed\": 1})</li>"
        "<li><strong>DELETE /tasks/:id</strong> - Deleta tarefa</li>"
        "</ul></body></html>";

    send_response(cliente, resposta);
}

// Rota: GET /tasks
static void route_get_tasks(SOCKET cliente)
{
    int count;
    Task *tasks = db_get_all_tasks(&count);

    if (count == 0)
    {
        const char *resposta =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "\r\n"
            "{\"tasks\": []}";
        send_response(cliente, resposta);
        return;
    }

    char json_body[8192] = "{\"tasks\": [";

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

    char http_response[10000];
    snprintf(http_response, sizeof(http_response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: application/json\r\n"
             "\r\n"
             "%s",
             json_body);

    send_response(cliente, http_response);
    free(tasks);
}

// Rota: GET /tasks/:id

static void route_get_task_by_id(SOCKET cliente, int task_id)
{
    Task *task = db_get_task_by_id(task_id);

    char json_resposta[1024];
    if (task)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"id\": %d, \"title\": \"%s\", \"completed\": %d}",
                 task->id, task->title, task->completed);
        free(task);
    }
    else
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"Tarefa nao encontrada\"}");
    }

    send - response(cliente, json_resposta);
}

// Rota: POST /tasks

static void route_create_task(SOCKET cliente, const char *body)
{
    TaskData data = parse_task_json(body);

    char json_resposta[1024];

    if (!data.valid || strlen(data.title) == 0)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 400 Bad Request\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"JSON invalido ou campo 'title' ausente\"}");
        send_response(cliente, json_resposta);
        return;
    }

    int novo_id = db_create_task(data.title, data.completed);

    if (novo_id > 0)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 201 Created\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"message\": \"Tarefa criada\", \"id\": %d, \"title\": \"%s\"}",
                 novo_id, data.title);
    }
    else
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 500 Internal Server Error\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"Erro ao criar tarefa\"}");
    }

    send_response(cliente, json_resposta);
}

// Rota: PUT /tasks/:id
// Rota: PUT /tasks/:id
static void route_update_task(SOCKET cliente, int task_id, const char *body)
{
    TaskData data = parse_task_json(body);

    char json_resposta[1024];

    if (!data.valid || strlen(data.title) == 0)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 400 Bad Request\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"JSON invalido ou campo 'title' ausente\"}");
        send_response(cliente, json_resposta);
        return;
    }

    int resultado = db_update_task(task_id, data.title, data.completed);

    if (resultado == 0)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"message\": \"Tarefa %d atualizada\", \"title\": \"%s\"}",
                 task_id, data.title);
    }
    else
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 500 Internal Server Error\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"Erro ao atualizar tarefa\"}");
    }

    send_response(cliente, json_resposta);
}

// Rota: DELETE /tasks/:id
static void route_delete_task(SOCKET cliente, int task_id)
{
    int resultado = db_delete_task(task_id);

    char json_resposta[512];
    if (resultado == 0)
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"message\": \"Tarefa %d deletada\"}",
                 task_id);
    }
    else
    {
        snprintf(json_resposta, sizeof(json_resposta),
                 "HTTP/1.1 500 Internal Server Error\r\n"
                 "Content-Type: application/json\r\n"
                 "\r\n"
                 "{\"error\": \"Erro ao deletar tarefa\"}");
    }

    send_response(cliente, json_resposta);
}

// Rota: 404
static void route_not_found(SOCKET cliente)
{
    const char *resposta =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "{\"error\": \"Rota nao encontrada\"}";

    send_response(cliente, resposta);
}

// Handler principal: Roteia requisições
void http_handle_request(SOCKET cliente, const char *requisicao)
{
    char metodo[10];
    char caminho[256];

    parse_request(requisicao, metodo, caminho);

    const char *body = extract_body(requisicao);

    // Roteamento
    if (strcmp(metodo, "GET") == 0 && strcmp(caminho, "/") == 0)
    {
        route_home(cliente);
    }
    else if (strcmp(metodo, "GET") == 0 && strcmp(caminho, "/tasks") == 0)
    {
        route_get_tasks(cliente);
    }
    else if (strcmp(metodo, "GET") == 0 && strncmp(caminho, "/tasks/", 7) == 0)
    {
        int task_id = atoi(caminho + 7);
        route_get_task_by_id(cliente, task_id);
    }
    else if (strcmp(metodo, "POST") == 0 && strcmp(caminho, "/tasks") == 0)
    {
        route_create_task(cliente, body);
    }
    else if (strcmp(metodo, "PUT") == 0 && strncmp(caminho, "/tasks/", 7) == 0)
    {
        int task_id = atoi(caminho + 7);
        route_update_task(cliente, task_id, body);
    }
    else if (strcmp(metodo, "DELETE") == 0 && strncmp(caminho, "/tasks/", 7) == 0)
    {
        int task_id = atoi(caminho + 7);
        route_delete_task(cliente, task_id);
    }
    else
    {
        route_not_found(cliente);
    }
}
