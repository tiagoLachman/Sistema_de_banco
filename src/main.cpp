#include <sqlite3.h>
#include <stdio.h>

#include <cliente.hpp>
#include <iostream>

void mostrarCliente(Cliente* cliente)
{
    if (!cliente->getLogado())
    {
        throw exception("Cliente nao logado\n");
    }
    printf("\nNome:%s\n", cliente->getNome().c_str());
    printf("Cpf:%s\n", cliente->getCpf().c_str());
    printf("Saldo:%lf\n", cliente->getSaldo());
    //printf("%i\n", cliente->getId());
}

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

    int op = 0;
    while (1)
    {
        try
        {
            printf("\n1->Adicionar cliente\n2->Logar na conta\n3->Sacar\n4->Depositar\n5->Mostrar conta\n6->Deslogar\n7->Sair\nR:");
            scanf("%d", &op);

            if (op == 1)
            {
                string nome;
                string cpf;
                string senha;

                printf("Nome:");
                cin >> nome;
                printf("Cpf:");
                cin >> cpf;
                printf("Senha:");
                cin >> senha;

                cliente.adicionar(nome, cpf, senha, false, 0.0);
            }
            else if (op == 2)
            {
                string cpf;
                string senha;

                printf("Cpf:");
                cin >> cpf;

                cliente.buscarPorCpf(cpf);

                printf("Senha:");
                cin >> senha;
                cliente.logar(senha);
            }
            else if (op == 3)
            {
                printf("Valor:");
                double valor;
                scanf("%lf", &valor);
                cliente.sacar(cliente.getCpf(), valor);
            }
            else if (op == 4)
            {
                printf("Valor:");
                double valor;
                scanf("%lf", &valor);
                cliente.depositar(cliente.getCpf(), valor);
            }
            else if (op == 5)
            {
                mostrarCliente(&cliente);
            }
            else if (op == 6)
            {
                cliente.deslogar();
            }
            else if (op == 7)
            {

                printf("Tenha uma otima semana!");
                break;
            }
            else
            {
                printf("Opcao invalida");
                break;
            }


        } catch (exception err)
        {
            printf("%s", err.what());
        }
    }

    sqlite3_close(db);
    return 0;
}
