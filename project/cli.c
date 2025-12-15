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




