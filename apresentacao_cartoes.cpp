/**
 * @file apresentacao_cartoes.cpp
 * @brief Implementacao da controladora de apresentacao de cartoes de atividade
 *
 * Cada campo digitado e validado pelo set do dominio dentro de try/catch.
 * Em caso de formato invalido, a mensagem e exibida e o campo e pedido novamente.
 * Nenhuma regra de negocio e aplicada aqui.
 * O momento de movimentacao e preenchido com timestampAgora() automaticamente.
 */

#include <iostream>
#include "apresentacao_cartoes.h"
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
        try { Identificador id; id.setValor(s); return id; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Identificador: 3 letras + 3 digitos (ex: ABC123).\n";
        }
    }
}

static Nome lerNome(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Nome n; n.setNome(s); return n; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Nome: 1-15 letras e espacos.\n";
        }
    }
}

static Texto lerTexto(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Texto t; t.setValor(s); return t; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Texto: ate 30 chars, inicia maiuscula, termina com ponto.\n";
        }
    }
}

static Prioridade lerPrioridade(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Prioridade p; p.setPrioridade(s); return p; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Prioridade: ALTA, MEDIA ou BAIXA.\n";
        }
    }
}

static Tamanho lerTamanho(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Tamanho t; t.setTamanho(s); return t; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Tamanho: GRANDE, MEDIO ou PEQUENO.\n";
        }
    }
}

static Estado lerEstado(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Estado e; e.setEstado(s); return e; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Estado: A FAZER, FAZENDO ou FEITO.\n";
        }
    }
}

static Email lerEmail(const string& prompt) {
    while (true) {
        string s = lerLinha(prompt);
        try { Email e; e.setEmail(s); return e; }
        catch (const invalid_argument&) {
            cout << "Formato invalido. Email: parte-local@dominio.\n";
        }
    }
}

// ===========================================================================
// CntrIUCartaoDeAtividade
// ===========================================================================

void CntrIUCartaoDeAtividade::setCntrServicosCartaoDeAtividade(IServicosCartaoDeAtividade* cntr) {
    cntrServicos = cntr;
}

bool CntrIUCartaoDeAtividade::criar(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Criar Cartao ---\n";
    Identificador id        = lerIdentificador("Identificador do cartao (3 letras + 3 digitos): ");
    Identificador idQuadro  = lerIdentificador("Identificador do quadro (3 letras + 3 digitos): ");
    Email emailDev          = lerEmail("Email do desenvolvedor: ");
    Nome nome               = lerNome("Nome do cartao: ");
    Texto desc              = lerTexto("Descricao: ");
    Prioridade prio         = lerPrioridade("Prioridade (ALTA, MEDIA, BAIXA): ");
    Tamanho tam             = lerTamanho("Tamanho (GRANDE, MEDIO, PEQUENO): ");

    CartaoDeAtividade c;
    c.setIdentificador(id);
    c.setNome(nome);
    c.setDescricao(desc);
    c.setPrioridade(prio);
    c.setTamanho(tam);

    // Estado inicial obrigatorio: A FAZER
    Estado est;
    est.setEstado("A FAZER");
    c.setEstado(est);

    // Entrada = timestamp atual (preenchido pela apresentacao)
    Timestamp agora;
    agora.setValor(timestampAgora());
    c.setEntrada(agora);
    // inicio e termino: placeholder igual a entrada (substituidos ao mover)
    c.setInicio(agora);
    c.setTermino(agora);

    if (!cntrServicos->criar(c, idQuadro, emailDev)) {
        cout << "Falha ao criar cartao. Verifique identificador, quadro e desenvolvedor.\n";
        return false;
    }
    cout << "Cartao criado com sucesso. Entrada: " << agora.getValor() << "\n";
    return true;
}

bool CntrIUCartaoDeAtividade::ler() {
    cout << "\n--- Ler Cartao ---\n";
    Identificador id = lerIdentificador("Identificador do cartao: ");
    CartaoDeAtividade c;
    c.setIdentificador(id);
    if (!cntrServicos->ler(&c)) {
        cout << "Cartao nao encontrado.\n";
        return false;
    }
    cout << "Identificador: " << c.getIdentificador().getValor()   << "\n";
    cout << "Nome         : " << c.getNome().getNome()             << "\n";
    cout << "Descricao    : " << c.getDescricao().getValor()       << "\n";
    cout << "Prioridade   : " << c.getPrioridade().getPrioridade() << "\n";
    cout << "Estado       : " << c.getEstado().getEstado()         << "\n";
    cout << "Tamanho      : " << c.getTamanho().getTamanho()       << "\n";
    cout << "Entrada      : " << c.getEntrada().getValor()         << "\n";
    cout << "Inicio       : " << c.getInicio().getValor()          << "\n";
    cout << "Termino      : " << c.getTermino().getValor()         << "\n";
    return true;
}

bool CntrIUCartaoDeAtividade::atualizar(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Atualizar Cartao ---\n";
    Identificador id = lerIdentificador("Identificador do cartao a atualizar: ");
    CartaoDeAtividade c;
    c.setIdentificador(id);
    if (!cntrServicos->ler(&c)) {
        cout << "Cartao nao encontrado.\n";
        return false;
    }
    Nome nome       = lerNome("Novo nome: ");
    Texto desc      = lerTexto("Nova descricao: ");
    Prioridade prio = lerPrioridade("Nova prioridade (ALTA, MEDIA, BAIXA): ");
    Tamanho tam     = lerTamanho("Novo tamanho (GRANDE, MEDIO, PEQUENO): ");
    c.setNome(nome);
    c.setDescricao(desc);
    c.setPrioridade(prio);
    c.setTamanho(tam);
    if (!cntrServicos->atualizar(c)) {
        cout << "Falha ao atualizar cartao.\n";
        return false;
    }
    cout << "Cartao atualizado com sucesso.\n";
    return true;
}

bool CntrIUCartaoDeAtividade::excluir(const Email& gestor) {
    (void)gestor;
    cout << "\n--- Excluir Cartao ---\n";
    Identificador id = lerIdentificador("Identificador do cartao a excluir: ");
    if (!cntrServicos->excluir(id)) {
        cout << "Falha ao excluir cartao.\n";
        return false;
    }
    cout << "Cartao excluido com sucesso.\n";
    return true;
}

bool CntrIUCartaoDeAtividade::listar() {
    cout << "\n--- Listar Cartoes ---\n";
    list<Identificador> lista;
    cntrServicos->listar(&lista);
    if (lista.empty()) {
        cout << "Nenhum cartao cadastrado.\n";
        return true;
    }
    cout << "Cartoes:\n";
    for (const auto& id : lista) {
        cout << "  " << id.getValor() << "\n";
    }
    return true;
}

bool CntrIUCartaoDeAtividade::mover() {
    cout << "\n--- Mover Cartao ---\n";
    Identificador id    = lerIdentificador("Identificador do cartao: ");
    Estado        dest  = lerEstado("Estado destino (FAZENDO ou FEITO): ");
    // Momento = relogio do sistema (nao e pedido ao usuario)
    Timestamp momento;
    momento.setValor(timestampAgora());
    if (!cntrServicos->mover(id, dest, momento)) {
        cout << "Falha ao mover cartao. Verifique estado atual, WIP ou timestamp.\n";
        return false;
    }
    cout << "Cartao movido para " << dest.getEstado()
         << " em " << momento.getValor() << ".\n";
    return true;
}

bool CntrIUCartaoDeAtividade::lerTempoCiclo() {
    cout << "\n--- Tempo de Ciclo ---\n";
    Identificador id = lerIdentificador("Identificador do cartao: ");
    double minutos;
    if (!cntrServicos->lerTempoCiclo(id, minutos)) {
        cout << "Cartao nao encontrado ou nao esta em FEITO.\n";
        return false;
    }
    cout << "Tempo de ciclo: " << minutos << " minuto(s).\n";
    return true;
}

bool CntrIUCartaoDeAtividade::lerTempoCicloMedio() {
    cout << "\n--- Tempo de Ciclo Medio ---\n";
    Identificador id = lerIdentificador("Identificador do quadro: ");
    double minutos;
    if (!cntrServicos->lerTempoCicloMedio(id, minutos)) {
        cout << "Quadro nao encontrado ou sem cartoes FEITO.\n";
        return false;
    }
    cout << "Tempo de ciclo medio: " << minutos << " minuto(s).\n";
    return true;
}

bool CntrIUCartaoDeAtividade::lerLeadTime() {
    cout << "\n--- Lead Time ---\n";
    Identificador id = lerIdentificador("Identificador do cartao: ");
    double minutos;
    if (!cntrServicos->lerLeadTime(id, minutos)) {
        cout << "Cartao nao encontrado ou nao esta em FEITO.\n";
        return false;
    }
    cout << "Lead time: " << minutos << " minuto(s).\n";
    return true;
}

bool CntrIUCartaoDeAtividade::lerLeadTimeMedio() {
    cout << "\n--- Lead Time Medio ---\n";
    Identificador id = lerIdentificador("Identificador do quadro: ");
    double minutos;
    if (!cntrServicos->lerLeadTimeMedio(id, minutos)) {
        cout << "Quadro nao encontrado ou sem cartoes FEITO.\n";
        return false;
    }
    cout << "Lead time medio: " << minutos << " minuto(s).\n";
    return true;
}

bool CntrIUCartaoDeAtividade::lerWip() {
    cout << "\n--- WIP do Quadro ---\n";
    Identificador id = lerIdentificador("Identificador do quadro: ");
    int wip;
    if (!cntrServicos->lerWip(id, wip)) {
        cout << "Quadro nao encontrado.\n";
        return false;
    }
    cout << "WIP atual: " << wip << " cartao(oes) em FAZENDO.\n";
    return true;
}

bool CntrIUCartaoDeAtividade::executar(const Email& email, const Papel& papel) {
    int opcao;
    do {
        cout << "\n=== CARTOES ===\n";
        if (papel.getPapel() == "GESTOR") {
            cout << " 1. Criar cartao\n";
            cout << " 2. Ler cartao\n";
            cout << " 3. Atualizar cartao\n";
            cout << " 4. Excluir cartao\n";
            cout << " 5. Listar cartoes\n";
            cout << " 6. Mover cartao\n";
            cout << " 7. Tempo de ciclo\n";
            cout << " 8. Tempo de ciclo medio\n";
            cout << " 9. Lead time\n";
            cout << "10. Lead time medio\n";
            cout << "11. WIP do quadro\n";
        } else {
            cout << "1. Ler cartao\n";
            cout << "2. Listar cartoes\n";
            cout << "3. Mover cartao\n";
            cout << "4. Tempo de ciclo\n";
            cout << "5. Tempo de ciclo medio\n";
            cout << "6. Lead time\n";
            cout << "7. Lead time medio\n";
            cout << "8. WIP do quadro\n";
        }
        cout << "0. Voltar\n";
        cout << "Opcao: ";
        string s;
        getline(cin, s);
        if (s.empty()) { opcao = -1; continue; }
        try { opcao = paraInt(s); } catch (...) { opcao = -1; continue; }

        if (papel.getPapel() == "GESTOR") {
            switch (opcao) {
                case  1: criar(email);         break;
                case  2: ler();                break;
                case  3: atualizar(email);     break;
                case  4: excluir(email);       break;
                case  5: listar();             break;
                case  6: mover();              break;
                case  7: lerTempoCiclo();      break;
                case  8: lerTempoCicloMedio(); break;
                case  9: lerLeadTime();        break;
                case 10: lerLeadTimeMedio();   break;
                case 11: lerWip();             break;
                case  0: break;
                default: cout << "Opcao invalida.\n";
            }
        } else {
            switch (opcao) {
                case 1: ler();                break;
                case 2: listar();             break;
                case 3: mover();              break;
                case 4: lerTempoCiclo();      break;
                case 5: lerTempoCicloMedio(); break;
                case 6: lerLeadTime();        break;
                case 7: lerLeadTimeMedio();   break;
                case 8: lerWip();             break;
                case 0: break;
                default: cout << "Opcao invalida.\n";
            }
        }
    } while (opcao != 0);
    return true;
}
