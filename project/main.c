/*
===============================================================================================
C Task Manager API - Gerenciador de tarefas com API REST em C puro
Autor: Marcos Daniel
GitHub: https://github.com/Daniel16Bit/c-task-manager-api
===============================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "database.h"
#include "cli.h"
#include "tests.h"

#define PORTA_PADRAO 8080

// Exibe menu de seleção de modo
static void show_mode_menu() {
    printf("\n");
    printf("========================================\n");
    printf("      C TASK MANAGER API v1.0          \n");
    printf("========================================\n");
    printf("\n");
    printf("Escolha o modo de execucao:\n\n");
    printf("1. Servidor HTTP (API REST)\n");
    printf("2. Interface CLI (Terminal)\n");
    printf("3. Executar Testes Automatizados\n");
    printf("0. Sair\n");
    printf("\n");
    printf("Opcao: ");
}

int main(int argc, char *argv[]) {
    // Verificar argumentos da linha de comando
    if (argc > 1) {
        if (strcmp(argv[1], "--server") == 0) {
            // Modo servidor direto
            printf("Iniciando em modo SERVIDOR...\n\n");
            
            if (db_init("tasks.db") != 0) {
                fprintf(stderr, "Erro ao inicializar banco de dados!\n");
                return 1;
            }
            
            SOCKET servidor = server_init(PORTA_PADRAO);
            if (servidor == INVALID_SOCKET) {
                db_close();
                return 1;
            }
            
            server_run(servidor);
            
            server_cleanup(servidor);
            db_close();
            return 0;
        }
        else if (strcmp(argv[1], "--cli") == 0) {
            // Modo CLI direto
            printf("Iniciando em modo CLI...\n\n");
            
            if (db_init("tasks.db") != 0) {
                fprintf(stderr, "Erro ao inicializar banco de dados!\n");
                return 1;
            }
            
            cli_start();
            
            db_close();
            return 0;
        }
        else if (strcmp(argv[1], "--test") == 0) {
            // Modo teste direto
            int failed = run_all_tests();
            return (failed == 0) ? 0 : 1;
        }
        else if (strcmp(argv[1], "--help") == 0) {
            printf("\nC Task Manager API - Uso:\n\n");
            printf("  server.exe              Menu interativo\n");
            printf("  server.exe --server     Iniciar servidor HTTP\n");
            printf("  server.exe --cli        Iniciar interface CLI\n");
            printf("  server.exe --test       Executar testes\n");
            printf("  server.exe --help       Mostrar esta ajuda\n\n");
            return 0;
        }
    }
    
    // Modo interativo (menu)
    int opcao;
    
    do {
        show_mode_menu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: {
                // Servidor HTTP
                printf("\nIniciando SERVIDOR HTTP...\n\n");
                
                if (db_init("tasks.db") != 0) {
                    fprintf(stderr, "Erro ao inicializar banco de dados!\n");
                    return 1;
                }
                
                SOCKET servidor = server_init(PORTA_PADRAO);
                if (servidor == INVALID_SOCKET) {
                    db_close();
                    return 1;
                }
                
                printf("Pressione Ctrl+C para encerrar o servidor.\n\n");
                
                server_run(servidor);
                
                server_cleanup(servidor);
                db_close();
                break;
            }
            
            case 2: {
                // CLI
                printf("\nIniciando INTERFACE CLI...\n\n");
                
                if (db_init("tasks.db") != 0) {
                    fprintf(stderr, "Erro ao inicializar banco de dados!\n");
                    return 1;
                }
                
                cli_start();
                
                db_close();
                printf("\nVoltando ao menu principal...\n");
                break;
            }
            
            case 3: {
                // Testes
                int failed = run_all_tests();
                
                if (failed == 0) {
                    printf("Todos os testes passaram! ✓\n");
                } else {
                    printf("%d teste(s) falharam! ✗\n", failed);
                }
                
                printf("\nPressione ENTER para voltar ao menu...");
                getchar();
                getchar();
                break;
            }
            
            case 0:
                printf("\nEncerrando programa. Ate logo!\n");
                break;
            
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("Pressione ENTER...");
                getchar();
                getchar();
        }
        
    } while (opcao != 0);
    
    return 0;
}