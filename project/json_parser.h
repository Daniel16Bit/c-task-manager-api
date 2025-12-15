#ifndef JSON_PARSER.H
#define JSON_PARSER .H

// Estrutura para dados de tarefa parseados de JSON
typedef struct
{
    char title[256];
    int completed;
    int valid;
} TaskData;

TaskData parse_task_json(const char *json);

#endif