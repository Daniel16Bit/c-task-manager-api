#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sqlite3 *db = NULL; //LEMBRETE -> static nesta situação serve para: limitar a chamada deste escopo para somente dentro da database.c

int db_init (const char *db_path){

    // Iniciando DB
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Erro ao Abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    


    //Criar tabela caso ela não exista!
    const char *sql_create =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "completed INTEGER DEFAULT 0);";

    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql_create, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Erro ao criar a tabela: %s\n", &err_msg);
        return -1;
    }

    return 0;
}

// Fecha o banco de dados
void db_close() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}


int db_create_task(const char *title, int completed){
    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO tasks (title, completed) VALUES ('%s', %d);",
             title, completed);

    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Erro ao criar tarefa %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
        return sqlite3_last_insert_rowid(db);

}

// Callback para contar linhas
static int count_callback(void *data, int argc, char **argv, char **col_names){
    int *count = (int*)data;
    (*count)++;
    return 0;
}

// Variáveis globais estáticas para preencher array
static Task *task_array = NULL;
static int task_index = 0;

// Callback para preencher array de tasks
static int tasks_callback(void *data, int argc, char **argv, char **col_names){
    Task *task = &task_array[task_index++];
    task->id = atoi(argv[0]);
    strncpy(task->title, argv[1], 255);
    task->title[255] = '\0';
    task->completed = atoi(argv[2]);
    return 0;
}

// Retorna todas as tarefas
Task* db_get_all_tasks(int *count){
    *count = 00;
    
    // Primeiro, contar quantas tarefas existem
    const char *sql_count = "SELECT * FROM Task;";
    sqlite3_exec(db, sql_count, count_callback, count, NULL);

    if (*count == 0){
        return NULL;
    }
    // Alocar memória para as tarefas
    task_array = (Task*)malloc(sizeof(Task) * (*count));
    task_index = 0;


    // Buscar as tarefas
    sqlite3_exec(db, sql_count, tasks_callback, NULL, NULL);

    return task_array;

}

Task* db_get_all_tasks_byId(int id){
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECET * FROM tasks WHERE id = %d;", id);

    int count = 0;
    sqlite3_exec(db, sql, count_callback, &count, NULL);

    if (count == 0){
        return NULL;
    }

    Task *task = (Task*)malloc(sizeof(Task));
    task_array = task;
    task_index = 0;

    sqlite3_exec(db, sql, tasks_callback, NULL,NULL);

    return task;
}


// Atualizando tarefa
int db_update_task(int id, const char *title, int completed){
    char sql[256];
    snprintf(sql, sizeof(sql),
             "UPDATE tasks SET title = '%s', completed = %d WHERE id = %d;",
             title, completed, id);
    

    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Erro ao atualizar: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;
}

// Deletar uma tarefa
int db_delete_task(int id){
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM tasks WHERE id = %d;", id);

    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Erro ao deletar: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;

}


