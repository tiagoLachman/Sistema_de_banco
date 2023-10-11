#ifndef CLIENTE_H
#define CLIENTE_H

#include <sqlite3.h>

#include <string>
using namespace std;

class Cliente
{
private:
    bool     dbSet;
    sqlite3* db;
    int      id;
    string   Nome;
    string   Cpf;
    double   Saldo;
    bool     Bloqueado;
    string   Senha;
    bool     logado;


public:
    Cliente();
    Cliente(sqlite3* db);

    string getNome()
    {
        return this->Nome;
    }
    string getCpf()
    {
        return this->Cpf;
    }
    int getId()
    {
        return this->id;
    }
    double getSaldo()
    {
        return this->Saldo;
    }

    void popObjecto(string field, char* value)
    {
        if (field == "Nome")
        {
            this->Nome = string((char*)value);
        }
        else if (field == "Cpf")
        {
            this->Cpf = string((char*)value);
        }
        else if (field == "Saldo")
        {
            this->Saldo = atof(value);
        }
        else if (field == "Bloqueado")
        {
            this->Bloqueado = value == "1";
        }
        else if (field == "id")
        {
            this->id = atoi(value);
        }
    }


    Cliente* buscarPorCpf(string cpf);
    Cliente* adicionar(string Nome, string Cpf, string Senha, bool Bloqueado, double Saldo);
    Cliente* atualizar(string Nome, string Cpf, string Senha, bool Bloqueado, double Saldo);


    void sacar(string Cpf, double valor);
    void logar(string senha);
    void deslogar();
};

#endif //CLIENTE_H