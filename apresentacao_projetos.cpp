/**
 * @file apresentacao_projetos.cpp
 * @brief Implementacao da controladora de apresentacao de projetos
 *
 * Cada campo digitado e validado pelo set do dominio dentro de try/catch.
 * Em caso de formato invalido, a mensagem e exibida e o campo e pedido novamente.
 * Nenhuma regra de negocio e aplicada aqui.
 */

#include <iostream>
#include "apresentacao_projetos.h"
#include "utilitarios.h"

using namespace std;

// ===========================================================================
// Helpers de leitura com validacao e repeticao
// ===========================================================================

static string lerLinha(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static Identificador lerIdentificador(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Identificador id;
            id.setValor(s);
            return id;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Identificador: 3 letras seguidas de 3 digitos (ex: ABC123).\n";
        }
    }
}

static Nome lerNome(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Nome n;
            n.setNome(s);
            return n;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Nome: 1-15 letras e espacos.\n";
        }
    }
}

static Texto lerTexto(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Texto t;
            t.setValor(s);
            return t;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Texto: ate 30 caracteres, inicia maiuscula, termina com ponto.\n";
        }
    }
}

static Timestamp lerTimestamp(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Timestamp ts;
            ts.setValor(s);
            return ts;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Timestamp: DIA-MES-ANO-HH:MM (ex: 1-JAN-2024-08:00).\n";
        }
    }
}

// ===========================================================================
// CntrIUProjetos
// ===========================================================================

void CntrIUProjetos::setCntrServicosProjetos(IServicosProjetos* cntr) {
    cntrServicos = cntr;
}

bool CntrIUProjetos::criar(const Email& gestor) {
    cout << "\n--- Criar Projeto ---\n";
    Identificador id  = lerIdentificador("Identificador (3 letras + 3 digitos): ");
    Nome nome         = lerNome("Nome: ");
    Texto desc        = lerTexto("Descricao: ");
    Timestamp ini     = lerTimestamp("Inicio (DIA-MES-ANO-HH:MM): ");
    Timestamp fim     = lerTimestamp("Termino (DIA-MES-ANO-HH:MM): ");
    Projeto p;
    p.setIdentificador(id);
    p.setNome(nome);
    p.setDescricao(desc);
    p.setInicio(ini);
    p.setTermino(fim);
    if (!cntrServicos->criar(p, gestor)) {
        cout << "Falha ao criar projeto. O identificador pode ja existir.\n";
        return false;
    }
    cout << "Projeto criado com sucesso.\n";
    return true;
}

bool CntrIUProjetos::ler() {
    cout << "\n--- Ler Projeto ---\n";
    Identificador id = lerIdentificador("Identificador do projeto: ");
    Projeto p;
    p.setIdentificador(id);
    if (!cntrServicos->ler(&p)) {
        cout << "Projeto nao encontrado.\n";
        return false;
    }
    cout << "Identificador: " << p.getIdentificador().getValor() << "\n";
    cout << "Nome         : " << p.getNome().getNome()           << "\n";
    cout << "Descricao    : " << p.getDescricao().getValor()     << "\n";
    cout << "Inicio       : " << p.getInicio().getValor()        << "\n";
    cout << "Termino      : " << p.getTermino().getValor()       << "\n";
    return true;
}

bool CntrIUProjetos::atualizar(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Atualizar Projeto ---\n";
    Identificador id = lerIdentificador("Identificador do projeto a atualizar: ");
    Projeto p;
    p.setIdentificador(id);
    if (!cntrServicos->ler(&p)) {
        cout << "Projeto nao encontrado.\n";
        return false;
    }
    Nome nome     = lerNome("Novo nome: ");
    Texto desc    = lerTexto("Nova descricao: ");
    Timestamp ini = lerTimestamp("Novo inicio (DIA-MES-ANO-HH:MM): ");
    Timestamp fim = lerTimestamp("Novo termino (DIA-MES-ANO-HH:MM): ");
    p.setNome(nome);
    p.setDescricao(desc);
    p.setInicio(ini);
    p.setTermino(fim);
    if (!cntrServicos->atualizar(p)) {
        cout << "Falha ao atualizar projeto.\n";
        return false;
    }
    cout << "Projeto atualizado com sucesso.\n";
    return true;
}

bool CntrIUProjetos::excluir(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Excluir Projeto ---\n";
    Identificador id = lerIdentificador("Identificador do projeto a excluir: ");
    if (!cntrServicos->excluir(id)) {
        cout << "Falha ao excluir. O projeto pode ter quadros vinculados.\n";
        return false;
    }
    cout << "Projeto excluido com sucesso.\n";
    return true;
}

bool CntrIUProjetos::listar() {
    cout << "\n--- Listar Projetos ---\n";
    list<Identificador> lista;
    cntrServicos->listar(&lista);
    if (lista.empty()) {
        cout << "Nenhum projeto cadastrado.\n";
        return true;
    }
    cout << "Projetos:\n";
    for (const auto& id : lista) {
        cout << "  " << id.getValor() << "\n";
    }
    return true;
}

bool CntrIUProjetos::executar(const Email& email, const Papel& papel) {
    int opcao;
    do {
        cout << "\n=== PROJETOS ===\n";
        if (papel.getPapel() == "GESTOR") {
            cout << "1. Criar projeto\n";
            cout << "2. Ler projeto\n";
            cout << "3. Atualizar projeto\n";
            cout << "4. Excluir projeto\n";
            cout << "5. Listar projetos\n";
        } else {
            cout << "1. Ler projeto\n";
            cout << "2. Listar projetos\n";
        }
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        string s;
        getline(cin, s);
        if (s.empty()) { opcao = -1; continue; }
        try { opcao = paraInt(s); } catch (...) { opcao = -1; continue; }

        if (papel.getPapel() == "GESTOR") {
            switch (opcao) {
                case 1: criar(email);     break;
                case 2: ler();            break;
                case 3: atualizar(email); break;
                case 4: excluir(email);   break;
                case 5: listar();         break;
                case 0: break;
                default: cout << "Opcao invalida.\n";
            }
        } else {
            switch (opcao) {
                case 1: ler();    break;
                case 2: listar(); break;
                case 0: break;
                default: cout << "Opcao invalida.\n";
            }
        }
    } while (opcao != 0);
    return true;
}
