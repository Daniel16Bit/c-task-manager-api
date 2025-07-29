// test_sqlite.c
#include <sqlite3.h>
#include <stdio.h>

int main() {
    printf("SQLite version: %s\n", sqlite3_libversion());
    return 0;
}
