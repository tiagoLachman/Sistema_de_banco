#include <stdio.h>

#include <cliente.hpp>
#include <format>
#include <iostream>

template <typename... Args>
std::string string_format(const std::string& format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);

    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

int callback(void* cliente, int argc, char** argv, char** azColName)
{

    Cliente* c = (Cliente*)cliente;

    for (int i = 0; i < argc; i++)
    {
        c->popObjecto(string(azColName[i]), argv[i]);
    }

    return 0;
}

Cliente::Cliente(sqlite3* db)
{
    this->db    = db;
    this->dbSet = true;
    this->logado = false;
}

Cliente* Cliente::buscarPorCpf(string cpf)
{
    string sql;

    sql                   = string_format("SELECT id,Nome,Cpf,Saldo,Bloqueado,Senha FROM Clientes c WHERE c.Cpf = '%s'", cpf.c_str());
    char*         err_msg = 0;
    sqlite3_stmt* stmt    = NULL;


    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    //int   rc      = sqlite3_exec(db, sql.c_str(), callback, this, &err_msg);
    if (rc == SQLITE_OK)
    {
    }
    else
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
    int result = sqlite3_step(stmt);

    if (result == SQLITE_ROW)
    {
        int cont        = 0;
        this->id        = sqlite3_column_int(stmt, cont++);
        this->Nome      = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, cont++)));
        this->Cpf       = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, cont++)));
        this->Saldo     = sqlite3_column_double(stmt, cont++);
        this->Bloqueado = sqlite3_column_int(stmt, cont++);
        this->Senha     = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, cont++)));
    }

    sqlite3_finalize(stmt);

    return this;
}

Cliente* Cliente::adicionar(string Nome, string Cpf, string Senha, bool Bloqueado, double Saldo)
{
    string sql;
    sql = string_format("INSERT INTO Clientes(Cpf, Saldo, Nome, Bloqueado, Senha) VALUES(\'%s\', %lf, \'%s\', %i, '%s')", Cpf.c_str(), Saldo, Nome.c_str(), Bloqueado, Senha.c_str());

    char* err_msg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

    switch (rc)
    {
        case SQLITE_CONSTRAINT:
            throw exception("Cliente ja possui cadastro");
            break;
    }
    if (rc != SQLITE_OK)
    {
        throw exception(err_msg);
    }
    this->Nome      = Nome;
    this->Cpf       = Cpf;
    this->Bloqueado = Bloqueado;
    this->Saldo     = Saldo;
    return this;
}

Cliente* Cliente::atualizar(string Nome, string Cpf, string Senha, bool Bloqueado, double Saldo)
{
    int idCliente = buscarPorCpf(Cpf)->id;
    if (Nome.length() <= 0)
    {
        Nome = this->Nome;
    }
    if (Cpf.length() <= 0)
    {
        Cpf = this->Cpf;
    }
    string sql;
    sql = string_format("UPDATE Clientes SET Nome='%s', Saldo= %lf, Bloqueado=%d, Cpf='%s', Senha='%s' WHERE id=%d", Nome.c_str(), Saldo, Bloqueado, Cpf.c_str(), Senha.c_str(), idCliente);

    char* err_msg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);


    if (rc != SQLITE_OK)
    {
        throw exception(err_msg);
    }
    this->Nome      = Nome;
    this->Cpf       = Cpf;
    this->Bloqueado = Bloqueado;
    this->Saldo     = Saldo;
    return this;
}


void Cliente::sacar(string Cpf, double valor)
{
    buscarPorCpf(Cpf);
    if (this->Bloqueado)
    {
        throw exception("Conta bloqueada");
    }
    if (!this->logado)
    {
        throw exception("Conta nao logada");
    }
    if (valor > this->Saldo)
    {
        throw exception("Sem saldo");
    }

    atualizar(this->Nome, this->Cpf, this->Senha, this->Bloqueado, this->Saldo - valor);
}

void Cliente::depositar(string Cpf, double valor)
{
    buscarPorCpf(Cpf);
    if (this->Bloqueado)
    {
        throw exception("Conta bloqueada");
    }
    if (!this->logado)
    {
        throw exception("Conta nao logada");
    }
    if (valor <= 0)
    {
        throw exception("Não e possivel depositar esse valor");
    }

    atualizar(this->Nome, this->Cpf, this->Senha, this->Bloqueado, this->Saldo + valor);
}

void Cliente::logar(string senha){
    if(this->Senha == senha){
        this->logado = true;
        return;
    }
    throw exception("Senha incorreta");
}

void Cliente::deslogar(){
    this->logado = false;
}