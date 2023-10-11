#include <sqlite3.h>
#include <stdio.h>

#include <cliente.hpp>
#include <iostream>

int main()
{
    int      rc;
    sqlite3* db     = nullptr;
    char*    dbFile = "banco.db";

    rc = sqlite3_open(dbFile, &db);
    if (rc != SQLITE_OK)
    {
        printf("Erro ao abrir banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    Cliente cliente(db);
    try
    {
        //cliente.adicionar(string("nao"), string("12345"), "123Pin", false, 23.5);
        cliente.buscarPorCpf("123");
        cliente.logar("3123");
        cliente.sacar(cliente.getCpf(), 10.0);
        printf("%s\n", cliente.getNome().c_str());
        printf("%s\n", cliente.getCpf().c_str());
        printf("%lf\n", cliente.getSaldo());
        printf("%i\n", cliente.getId());
    } catch (exception err)
    {
        printf("%s", err.what());
    }

    sqlite3_close(db);
    return 0;
}
