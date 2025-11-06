#include <stdio.h>
#include "sqlite/sqlite3.h"

int main()
{

    sqlite3 *db;
    int rc = sqlite3_open("teste.db", &db);

    if (rc){
        printf("Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    printf("Banco de dados aberto com sucesso!\n");
    
    sqlite3_close(db);

return 0;
}