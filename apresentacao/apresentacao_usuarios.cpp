/**
 * @file apresentacao_usuarios.cpp
 * @brief Implementacao das controladoras de apresentacao de autenticacao e de pessoas
 *
 * Cada campo digitado e validado pelo set do dominio dentro de try/catch.
 * Em caso de formato invalido, a mensagem de erro e exibida e o campo e
 * pedido novamente (loop de repeticao). Nenhuma regra de negocio e aplicada aqui.
 */

#include <iostream>
#include "apresentacao/apresentacao_usuarios.h"
#include "utilitarios/utilitarios.h"

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

/** Le email valido; repete ate acertar o formato. */
static Email lerEmail(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Email e;
            e.setEmail(s);
            return e;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Email: parte-local@dominio (apenas minusculas/digitos/.-)\n";
        }
    }
}

/** Le senha valida; repete ate acertar o formato. */
static Senha lerSenha(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Senha v;
            v.setSenha(s);
            return v;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Senha: 5 caracteres alfanumericos, >= 1 letra e >= 1 digito.\n";
        }
    }
}

/** Le nome valido; repete ate acertar o formato. */
static Nome lerNome(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Nome n;
            n.setNome(s);
            return n;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Nome: 1-15 letras e espacos, sem iniciar/terminar com espaco.\n";
        }
    }
}

/** Le papel valido; repete ate acertar o formato. */
static Papel lerPapel(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            Papel p;
            p.setPapel(s);
            return p;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Papel: GESTOR ou DESENVOLVEDOR\n";
        }
    }
}

// ===========================================================================
// CntrIUAutenticacao
// ===========================================================================

void CntrIUAutenticacao::setCntrServicosAutenticacao(IServicosAutenticacao* cntr) {
    cntrServicos = cntr;
}

bool CntrIUAutenticacao::autenticar(Email* emailSaida, Papel* papelSaida) {
    cout << "\n=== AUTENTICACAO ===\n";
    Email e = lerEmail("Email: ");
    Senha s = lerSenha("Senha: ");
    Papel p;
    if (!cntrServicos->autenticar(e, s, &p)) {
        cout << "Email ou senha incorretos.\n";
        return false;
    }
    *emailSaida = e;
    *papelSaida = p;
    cout << "Autenticado como " << p.getPapel() << ".\n";
    return true;
}

// ===========================================================================
// CntrIUPessoas
// ===========================================================================

void CntrIUPessoas::setCntrServicosPessoas(IServicosPessoas* cntr) {
    cntrServicos = cntr;
}

bool CntrIUPessoas::cadastrar() {
    cout << "\n=== CRIAR CONTA ===\n";
    Email e = lerEmail("Email: ");
    Nome  n = lerNome("Nome: ");
    Senha s = lerSenha("Senha: ");
    Papel p = lerPapel("Papel (GESTOR ou DESENVOLVEDOR): ");
    Pessoa pessoa;
    pessoa.setEmail(e);
    pessoa.setNome(n);
    pessoa.setSenha(s);
    pessoa.setPapel(p);
    if (!cntrServicos->criar(pessoa)) {
        cout << "Falha: email ja cadastrado.\n";
        return false;
    }
    cout << "Conta criada com sucesso.\n";
    return true;
}

bool CntrIUPessoas::criar(const Email& usuarioLogado) {
    (void)usuarioLogado;
    return cadastrar();
}

bool CntrIUPessoas::ler(const Email& usuarioLogado, const Papel& papel) {
    Email alvo;
    if (papel.getPapel() == "GESTOR") {
        alvo = lerEmail("Email da pessoa: ");
    } else {
        alvo = usuarioLogado;
    }
    Pessoa p;
    p.setEmail(alvo);
    if (!cntrServicos->ler(&p)) {
        cout << "Pessoa nao encontrada.\n";
        return false;
    }
    cout << "\nEmail : " << p.getEmail().getEmail() << "\n";
    cout << "Nome  : " << p.getNome().getNome()   << "\n";
    cout << "Papel : " << p.getPapel().getPapel() << "\n";
    return true;
}

bool CntrIUPessoas::atualizar(const Email& usuarioLogado, const Papel& papel) {
    Email alvo;
    if (papel.getPapel() == "GESTOR") {
        alvo = lerEmail("Email da pessoa a atualizar: ");
    } else {
        alvo = usuarioLogado;
    }
    Pessoa p;
    p.setEmail(alvo);
    if (!cntrServicos->ler(&p)) {
        cout << "Pessoa nao encontrada.\n";
        return false;
    }
    Nome  n = lerNome("Novo nome: ");
    Senha s = lerSenha("Nova senha: ");
    Papel pv = lerPapel("Novo papel (GESTOR ou DESENVOLVEDOR): ");
    p.setNome(n);
    p.setSenha(s);
    p.setPapel(pv);
    if (!cntrServicos->atualizar(p)) {
        cout << "Falha ao atualizar.\n";
        return false;
    }
    cout << "Pessoa atualizada com sucesso.\n";
    return true;
}

bool CntrIUPessoas::excluir(const Email& usuarioLogado, const Papel& papel) {
    Email alvo;
    if (papel.getPapel() == "GESTOR") {
        alvo = lerEmail("Email da pessoa a excluir: ");
    } else {
        alvo = usuarioLogado;
    }
    if (!cntrServicos->excluir(alvo)) {
        cout << "Falha ao excluir. A pessoa pode ter projetos ou cartoes vinculados.\n";
        return false;
    }
    cout << "Pessoa excluida com sucesso.\n";
    return true;
}

bool CntrIUPessoas::listar() {
    list<Email> lista;
    cntrServicos->listar(&lista);
    if (lista.empty()) {
        cout << "Nenhuma pessoa cadastrada.\n";
        return true;
    }
    cout << "Pessoas:\n";
    for (const auto& e : lista) {
        cout << "  " << e.getEmail() << "\n";
    }
    return true;
}

bool CntrIUPessoas::executar(const Email& usuario, const Papel& papel) {
    int opcao;
    do {
        cout << "\n=== PESSOAS ===\n";
        if (papel.getPapel() == "GESTOR") {
            cout << "1. Criar pessoa\n";
            cout << "2. Ler pessoa\n";
            cout << "3. Atualizar pessoa\n";
            cout << "4. Excluir pessoa\n";
            cout << "5. Listar pessoas\n";
        } else {
            cout << "1. Ver meus dados\n";
            cout << "2. Atualizar meus dados\n";
            cout << "3. Excluir minha conta\n";
            cout << "4. Listar pessoas\n";
        }
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        string s;
        getline(cin, s);
        if (s.empty()) { opcao = -1; continue; }
        try { opcao = paraInt(s); } catch (...) { opcao = -1; continue; }

        if (papel.getPapel() == "GESTOR") {
            switch (opcao) {
                case 1: criar(usuario);           break;
                case 2: ler(usuario, papel);       break;
                case 3: atualizar(usuario, papel); break;
                case 4: excluir(usuario, papel);   break;
                case 5: listar();                  break;
                case 0: break;
                default: cout << "Opcao invalida.\n";
            }
        } else {
            switch (opcao) {
                case 1: ler(usuario, papel);       break;
                case 2: atualizar(usuario, papel); break;
                case 3: excluir(usuario, papel);   return true; // desloga
                case 4: listar();                  break;
                case 0: break;
                default: cout << "Opcao invalida.\n";
            }
        }
    } while (opcao != 0);
    return true;
}
