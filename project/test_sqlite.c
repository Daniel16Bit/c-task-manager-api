#include <stdio.h>
#include "sqlite/sqlite3.h"

// Função callback para processar resultados do SELECT
int callback(void *data, int argc, char **argv, char **col_names)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", col_names[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main()
{
    sqlite3 *db;
    char *err_msg = NULL;

    int rc = sqlite3_open("tasks.db", &db);
    if (rc)
    {
        printf("Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    printf("Banco de dados aberto!\n");

    // Criar tabela
    const char *sql_create_table =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL, "
        "completed INTEGER DEFAULT 0);";

    sqlite3_exec(db, sql_create_table, NULL, NULL, &err_msg);
    printf("Tabela verificada!\n");

    // Inserir dados (só se não existirem)
    const char *sql_insert =
        "INSERT INTO tasks (title, completed) VALUES "
        "('Aprender C', 0), "
        "('Criar API REST', 0), "
        "('Integrar SQLite', 1);";

    sqlite3_exec(db, sql_insert, NULL, NULL, &err_msg);

    // ===== LER DADOS =====
    printf("\n=== Lista de Tarefas ===\n");
    const char *sql_select = "SELECT * FROM tasks;";

    rc = sqlite3_exec(db, sql_select, callback, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("Erro ao ler dados: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // UPDATE - Marcar tarefa 1 como completa
    printf("\n=== Atualizando tarefa 1 ===\n");

    const char *sql_update =
        "UPDATE tasks SET completed = 1, title = 'Aprender C - COMPLETO' WHERE id = 1;";

    rc = sqlite3_exec(db, sql_update, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("Erro ao atualizar: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Tarefa 1 atualizada!\n");
    }

    // Listar novamente para ver a mudança
    printf("\n=== Lista Atualizada ===\n");
    sqlite3_exec(db, sql_select, callback, NULL, &err_msg);

    // DELETE - Remover tarefa 2
    printf("\n=== Deletando tarefa 2 ===\n");
    const char *sql_delete = "DELETE FROM tasks WHERE id = 2;";

    rc = sqlite3_exec(db, sql_delete, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("Erro ao deletar: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Tarefa 2 deletada!\n");
    }

        // Listar novamente para confirmar deleção
printf("\n=== Lista Final ===\n");
sqlite3_exec(db, sql_select, callback, NULL, &err_msg);

// ===== FIM  =====
sqlite3_close(db);
return 0;
}