#include "cli.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Limpa a tela (Windows)
static void clear_screen(){
    system("cls");
}

// Pausa e aguarda ENTER
static void pause_screen(){
    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar(); // Dois getchar para consumir \n residual

}


// Exibe menu principal
static void show_menu() {
    clear_screen();
    printf("========================================\n");
    printf("      C TASK MANAGER - CLI v1.0        \n");
    printf("========================================\n");
    printf("\n");
    printf("1. Listar todas as tarefas\n");
    printf("2. Buscar tarefa por ID\n");
    printf("3. Criar nova tarefa\n");
    printf("4. Atualizar tarefa\n");
    printf("5. Deletar tarefa\n");
    printf("0. Sair\n");
    printf("\n");
    printf("Escolha uma opcao: ");
}

//Opção 01: Listas todas
static void cli_list_task(){
    clear_screen();
    printf("=== LISTAR TODAS AS TAREFAS ===\n\n");

    int count;
    Task *tasks = db_get_all_tasks(&count);

    if (count == 0){
        printf("Nenhuma tarefa encontrada. \n");
    }else{
        printf("Total: %d tarefa(s)\n\n", count);
        printf("%-5s %-40s %-10s\n", "ID", "Titulo", "Status");
        printf("------------------------------------------------------------\n");

        for(int i = 0; i < count; i++){
            printf("%-5d %-40s %-10s\n",
                tasks[i].id,
                tasks[i].title,
                tasks[i].completed ? "Completa" : "Pendente");
        }
                free(tasks);
    }
    pause_screen();
}

//Opção 03: Buscar por ID

static void cli_get_id(){
    clear_screen();
    printf("=== BUSCAR TAREFA POR ID ===\n\n");

    int id;
    printf("Digite o ID da tarefa: ");
    scanf("%d, &id");

    Task *task = db_get_task_by_id(id);

    if (task) {
        printf("\n--- Tarefa Encontrada ---\n");
        printf("ID:        %d\n", task->id);
        printf("Titulo:    %s\n", task->title);
        printf("Status:    %s\n", task->completed ? "Completa" : "Pendente");
        printf("Completed: %d\n", task->completed);
        free(task);
    }else{
        printf("\nTarefa com ID %D não encontrada.\n", id);
    }
    pause_screen();
}

// Opção 03: Criar nova Task

static void cli_create_task(){
    clear_screen();
    printf("=== CRIAR NOVA TAREFA ===\n\n");

    char title[256];
    int completed;
}



