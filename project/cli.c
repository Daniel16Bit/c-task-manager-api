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

static void cli_get_task(){
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

    printf("Titulo da tarefa: ");
    getchar();
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0; // REMOVE O \n

    printf("Tarefa completa? (0 = Nao, 1 = Sim): ");
    scanf("%d", &completed);

    int novo_id = db_create_task(title, completed);

    if (novo_id > 0) {
        printf("\nSucesso! Tarefa criada com ID: %d\n", novo_id);
    } else {
        printf("\nErro ao criar tarefa.\n");
    }
    
    pause_screen();

}

// Opção 4: Atualizar

static void cli_update_task(){
    clear_screen();
    printf("=== ATUALIZAR TAREFA ===\n\n");

    int id;
    printf("Digite o ID da tarefa a atualizar: ");
    scanf("%d", &id);

    // Verificar se existe
    Task *task = db_get_task_by_id(id);
    if (!task) {
        printf("\nTarefa com ID %d nao encontrada.\n", id);
        pause_screen();
        return;
    }
   
    printf("\n--- Tarefa Atual ---\n");
    printf("Titulo: %s\n", task->title);
    printf("Status: %s\n\n", task->completed ? "Completa" : "Pendente");
    free(task);

    char new_title[256];
    int new_completed;

    printf("Novo titulo: ");
    getchar();
    fgets(new_title, sizeof(new_title), stdin);
    new_title[strcspn(new_title, "\n")] = 0;

    printf("Nova status (0 = Pendente, 1 = Completa): ");
    scanf("%d", &new_completed);

    int resultado = db_update_task(id, new_title, new_completed);

    if (resultado == 0) {
        printf("\nSucesso! Tarefa %d atualizada.\n", id);
    } else {
        printf("\nErro ao atualizar tarefa.\n");
    }
    
    pause_screen();
}


// Opção 5: Deletar

static void cli_delete_task(){
    clear_screen();
    printf("=== DELETAR TAREFA ===\n\n");

    int id;
    printf("Digite o ID da tarefa a deletar: ");
    scanf("%d", &id);
    
    // Verificar se existe
    Task *task = db_get_task_by_id(id);
    if (!task) {
        printf("\nTarefa com ID %d nao encontrada.\n", id);
        pause_screen();
        return;
    }

    printf("\n--- Tarefa a Deletar ---\n");
    printf("ID:     %d\n", task->id);
    printf("Titulo: %s\n\n", task->title);
    free(task);

    printf("Tem certeza? (1 = Sim, 0 = Nao): ");
    int confirma;
    scanf("%d", &confirma);

    if(confirma == 1){
        int resultado = db_delete_task(id);
        if(resultado == 0){
            printf("\nSucesso! Tarefa %d deletada.\n", id);
        }else{
            printf("\nErro ao deletar tarefa.\n");
        }
    }else{
        printf("\nOperacao cancelada.\n");
    }
    pause_screen();

}

// Loop principal da CLI
void cli_start(){
    int opcao;

    do{
        show_menu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cli_list_task();
            break;
        case 2:
            cli_get_task();
            break;
        case 3:
            cli_create_task();
            break;
        case 4:
            cli_update_task();
            break;
        case 5:
            cli_delete_task();
            break;
        case 0:
            printf("\nEncerrando Cli...\n");
            break;
        default:
            printf("\nOpcao invalida\n");
            break;

        }
    }while(opcao != 0);
}
