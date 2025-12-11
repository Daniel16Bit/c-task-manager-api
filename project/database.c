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