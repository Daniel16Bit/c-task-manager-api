#include "tests.h"
#include "database.h"
#include "json_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Contadores de testes
static int tests_passed = 0;
static int tests_failed = 0;

// Macro para asserção
#define ASSERT(condition, test_name) \
    if (condition) { \
        printf("  [PASS] %s\n", test_name); \
        tests_passed++; \
    } else { \
        printf("  [FAIL] %s\n", test_name); \
        tests_failed++; \
    }

// Teste 1: Banco de dados - Inicialização
static void test_db_init(){
    printf("\n=== Testando: Inicializacao do Banco ===\n");

    // Criar bando temporário
    int result = db_init("test_temp.db");
    ASSERT(result == 0, "db_initdeve retonar 0 em sucesso");

    db_close();
    remove("test_temp.db"); // limpar
}

// Teste 2: CRUD - Create
static void test_db_create(){
    printf("\n=== Testando: Criar Tarefa ===\n");

    db_init("test_temp.db");

    int id1 = db_create_task("Teste 1", 0);
    ASSERT(id1 > 0, "Criar tarefas deve retonar ID positivo");

    int id2 = db_create_task("Teste 2", 1);
    ASSERT(id2 == id1 + 1, "IDs devem ser sequenciais");

    db_close();
    remove("test_temp.db");
}

// Teste 3: CRUD - Read
static void test_db_read(){
    printf("\n=== Testando: Ler Tarefas ===\n");

    db_init("test_temp.db");

    db_create_task("Tarefa A", 0);
    db_create_task("Tarefa B", 1);

    int count;
    Task *tasks = db_get_all_tasks(&count);

    ASSERT(count == 2, "Deve retornar 2 tarefas");
    ASSERT(strcmp(tasks[0].title, "Tarefa A") == 0,  "Primeira tarefa deve ser 'Tarefa A'");
    ASSERT(tasks[1].completed == 1, "Segunda tarefa deve estar completa");

    free(tasks);

    Task *task_individual = db_get_task_by_id(1);
    ASSERT(task_individual != NULL, "Deve encontrar por ID");
    ASSERT(strcmp(task_individual->title, "Tarefa A") == 0, "Titulo deve ser 'Tarefa A'");
    free(task_individual);

    Task *inexistente = db_get_task_by_id(999);
    ASSERT(inexistente == NULL, "Tarefa inexistente deve retornar NULL");

    db_close();
    remove("test_temp.db");
}

// Teste 4: CRUD - Update
static void test_db_update(){
    printf("\n=== Testando: Atualizar Tarefa ===\n");
    
    db_init("test_temp.db");  

    int id = db_create_task("Original", 0);

    int resultado  = db_update_task(id, "Atualizado", 1);
    ASSERT(resultado == 0, "Update deve retornar 0 em sucesso");

    Task *task = db_get_task_by_id(id);
    ASSERT(strcmp(task->title, "Atualizada") == 0, "Titulo deve estar atualizado");
    ASSERT(task->completed == 1, "Status deve estar atualizado");
    free(task);

    db_close();
    remove("test_temp.db");
}

// Teste 5: CRUD - Delete
static void test_db_delete(){
    printf("\n=== Testando: Deletar Tarefa ===\n");

    db_init("test_temp.db");

    int id = db_create_task("Para deletar", 0);

    int resultado = db_delete_task(id);
    ASSERT(resultado == 0, "Delete deve retornar 0 em sucesso");

    Task *task = db_get_task_by_id(id);
    ASSERT(task == NULL, "Tarefa deletada nao deve ser encontrado");

    db_close();
    remove("test_temp.db");
}

// Teste 6: JSON Parser - Válido
static void test_json_parser_valid(){
    printf("\n=== Testando: Parser JSON Valido ===\n");

    const char *json1 = "{\"title\": \"Minha Tarefa\", \"completed\": 1}";
    TaskData data1 = parse_task_json(json1);

    ASSERT(data1.valid == 1, "JSON valido deve ser parseado");
    ASSERT(strcmp(data1.title, "Minha Tarefa") == 0, "Titulo deve ser extraido corretamente!");
    ASSERT(data1.completed == 1, "Completed deve ser 1");

    const char *json2 = "{\"title\": \"Sem completed\"}";
    TaskData data2 = parse_task_json(json2);
    
    ASSERT(data2.valid == 1, "JSON sem 'completed' deve ser valido");
    ASSERT(data2.completed == 0, "Completed deve ter valor padrao 0");
}

// Teste 7: JSON Parser - Inválido
static void test_json_parser_invalid(){
    printf("\n=== Testando: Parser JSON Invalido ===\n");

    const char *json_invalido = "{\"completed\": 1}";  // Sem title
    TaskData data = parse_task_json(json_invalido);

    ASSERT(data.valid == 0,"JSON sem 'title' deve ser invalido.");

    TaskData data_vazio = parse_task_json("");
    ASSERT(data_vazio.valid == 0, "String vazio deve ser invalido");

    TaskData data_null = parse_task_json(NULL);
    ASSERT(data_null.valid == 0, "NULL deve ser invalido");

}

// Executa todos os testes

int run_all_tests(){
    printf("\n");
    printf("========================================\n");
    printf("    EXECUTANDO TESTES AUTOMATIZADOS    \n");
    printf("========================================\n");

    tests_passed = 0;
    tests_failed = 0;

    // Rodas todos os testes
    test_db_init();
    test_db_create();
    test_db_read();
    test_db_update();
    test_db_delete();
    test_json_parser_valid();
    test_json_parser_invalid();

     // Resumo
    printf("\n========================================\n");
    printf("            RESUMO DOS TESTES           \n");
    printf("========================================\n");
    printf("Total:     %d testes\n", tests_passed + tests_failed);
    printf("Passaram:  %d (%.1f%%)\n", tests_passed, 
           (tests_passed * 100.0) / (tests_passed + tests_failed));
    printf("Falharam:  %d\n", tests_failed);
    printf("========================================\n\n");
    
    return tests_failed;
}
