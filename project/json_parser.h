#ifndef JSON_PARSER_H
#define JSON_PARSER_H

// Estrutura para dados de tarefa parseados de JSON
typedef struct
{
    char title[256];
    int completed;
    int valid;
} TaskData;

TaskData parse_task_json(const char *json);

#endif