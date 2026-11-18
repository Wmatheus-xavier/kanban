/**
 * @file apresentacao_quadros.cpp
 * @brief Implementacao da controladora de apresentacao de quadros
 *
 * Cada campo digitado e validado pelo set do dominio dentro de try/catch.
 * Em caso de formato invalido, a mensagem e exibida e o campo e pedido novamente.
 * Nenhuma regra de negocio e aplicada aqui.
 */

#include <iostream>
#include "apresentacao_quadros.h"
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

static Limite lerLimite(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try {
            int v = paraInt(s);
            Limite l;
            l.setLimite(v);
            return l;
        } catch (const invalid_argument&) {
            cout << "Formato invalido. Limite: numero inteiro de 1 a 25.\n";
        }
    }
}

// ===========================================================================
// CntrIUQuadros
// ===========================================================================

void CntrIUQuadros::setCntrServicosQuadros(IServicosQuadros* cntr) {
    cntrServicos = cntr;
}

bool CntrIUQuadros::criar(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Criar Quadro ---\n";
    Identificador id        = lerIdentificador("Identificador do quadro (3 letras + 3 digitos): ");
    Identificador idProjeto = lerIdentificador("Identificador do projeto (3 letras + 3 digitos): ");
    Nome nome               = lerNome("Nome: ");
    Limite limite           = lerLimite("Limite WIP (1-25): ");
    Quadro q;
    q.setIdentificador(id);
    q.setNome(nome);
    q.setLimite(limite);
    if (!cntrServicos->criar(q, idProjeto)) {
        cout << "Falha ao criar quadro. O identificador pode ja existir ou o projeto e inexistente.\n";
        return false;
    }
    cout << "Quadro criado com sucesso.\n";
    return true;
}

bool CntrIUQuadros::ler() {
    cout << "\n--- Ler Quadro ---\n";
    Identificador id = lerIdentificador("Identificador do quadro: ");
    Quadro q;
    q.setIdentificador(id);
    if (!cntrServicos->ler(&q)) {
        cout << "Quadro nao encontrado.\n";
        return false;
    }
    cout << "Identificador: " << q.getIdentificador().getValor() << "\n";
    cout << "Nome         : " << q.getNome().getNome()           << "\n";
    cout << "Limite WIP   : " << q.getLimite().getLimite()       << "\n";
    return true;
}

bool CntrIUQuadros::atualizar(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Atualizar Quadro ---\n";
    Identificador id = lerIdentificador("Identificador do quadro a atualizar: ");
    Quadro q;
    q.setIdentificador(id);
    if (!cntrServicos->ler(&q)) {
        cout << "Quadro nao encontrado.\n";
        return false;
    }
    Nome   nome   = lerNome("Novo nome: ");
    Limite limite = lerLimite("Novo limite WIP (1-25): ");
    q.setNome(nome);
    q.setLimite(limite);
    if (!cntrServicos->atualizar(q)) {
        cout << "Falha ao atualizar. O novo limite pode ser menor que o WIP atual.\n";
        return false;
    }
    cout << "Quadro atualizado com sucesso.\n";
    return true;
}

bool CntrIUQuadros::excluir(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Excluir Quadro ---\n";
    Identificador id = lerIdentificador("Identificador do quadro a excluir: ");
    if (!cntrServicos->excluir(id)) {
        cout << "Falha ao excluir. O quadro pode ter cartoes vinculados.\n";
        return false;
    }
    cout << "Quadro excluido com sucesso.\n";
    return true;
}

bool CntrIUQuadros::listar() {
    cout << "\n--- Listar Quadros ---\n";
    list<Identificador> lista;
    cntrServicos->listar(&lista);
    if (lista.empty()) {
        cout << "Nenhum quadro cadastrado.\n";
        return true;
    }
    cout << "Quadros:\n";
    for (const auto& id : lista) {
        cout << "  " << id.getValor() << "\n";
    }
    return true;
}

bool CntrIUQuadros::executar(const Email& email, const Papel& papel) {
    int opcao;
    do {
        cout << "\n=== QUADROS ===\n";
        if (papel.getPapel() == "GESTOR") {
            cout << "1. Criar quadro\n";
            cout << "2. Ler quadro\n";
            cout << "3. Atualizar quadro\n";
            cout << "4. Excluir quadro\n";
            cout << "5. Listar quadros\n";
        } else {
            cout << "1. Ler quadro\n";
            cout << "2. Listar quadros\n";
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
