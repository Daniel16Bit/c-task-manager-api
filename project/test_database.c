#include <stdio.h>
#include <stdlib.h>
#include "database.h"

int main(){
    printf("TESTANDO DATABASE");

    // Inicializando banco de dados

    if(db_init("tasks.db") != 0){
            printf("Erro ao iniciar o bando de dados!\n");
            return 1;
    }

    printf("Bando de dados INICIADO!!");

    //criar tarefas
    int id1 = db_create_task("Estudar C", 0);
    int id2 = db_create_task("Fazer API", 0);
    int id3 = db_create_task("Integrar SQLite", 1);
    printf("Tarefas criadas com sucesso! %d, %d, %d\n\n", id1, id2,id3);

//Listar todas
int count;
    Task *tasks = db_get_all_tasks(&count);
    printf("=== Todas as tarefas (%d) ===\n", count);
    for (int i = 0; i < count; i++) {
         printf("  ID: %d | %s | Completa: %d\n", 
        tasks[i].id, tasks[i].title, tasks[i].completed);
    }
    free(tasks);

// Buscar por Id
    printf("\n=== Buscando tarefa ID 2 ===\n");
    Task *task = db_get_all_tasks_byId(2);
    if (task) {
        printf("  %s | Completa: %d\n", task->title, task->completed);
        free(task);
    }
// Atualizar
    printf("\n=== Atualizando tarefa 1 ===\n");
    db_update_task(1, "Estudar C - COMPLETO", 1);
    printf("✓ Atualizada!\n");
    
// Deletar
    printf("\n=== Deletando tarefa 2 ===\n");
    db_delete_task(2);
    printf("✓ Deletada!\n");



// Listar novamente
    tasks = db_get_all_tasks(&count);
    printf("\n=== Lista final (%d tarefas) ===\n", count);
    for (int i = 0; i < count; i++) {
        printf("  ID: %d | %s | Completa: %d\n", 
               tasks[i].id, tasks[i].title, tasks[i].completed);
    }
    free(tasks);
    
    db_close();
    printf("\n✓ Banco fechado!\n");
    
    printf("\nPressione ENTER para sair...\n");  
    getchar();                                     
    
    return 0;
}