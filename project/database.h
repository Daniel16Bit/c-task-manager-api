#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite/sqlite3.h"

// Estruct feita com typedef (alavra-chave para criar um apelido (alias) para um tipo de dado existente) para representar uma tarefa.

typedef struct {
    int id;
    char title[256];
    int completed;
}Task;

// database function

int db_init(const char *db_path);
void db_close();
int db_create_task(const char *title, int completed);

Task* db_get_all_tasks(int *count);
Task* db_get_all_tasks_byId(int id);

int db_update_task(int id, const char *title, int completed);
int db_delete_task(int id);

#endif