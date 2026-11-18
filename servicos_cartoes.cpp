/**
 * @file servicos_cartoes.cpp
 * @brief Implementacao de ContainerCartoes e CntrServicosCartaoDeAtividade
 */

#include "servicos_cartoes.h"
#include "servicos_quadros.h"
#include "servicos_usuarios.h"
#include "utilitarios.h"

using namespace std;

// ---------------------------------------------------------------------------
// ContainerCartoes
// ---------------------------------------------------------------------------

bool ContainerCartoes::incluir(const CartaoDeAtividade& cartao, const Identificador& quadro,
                                const Email& desenvolvedor) {
    string chave = cartao.getIdentificador().getValor();
    if (dados.count(chave)) return false;
    dados[chave]           = cartao;
    quadros[chave]         = quadro.getValor();
    desenvolvedores[chave] = desenvolvedor.getEmail();
    return true;
}

bool ContainerCartoes::pesquisar(CartaoDeAtividade* cartao) const {
    string chave = cartao->getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    *cartao = it->second;
    return true;
}

bool ContainerCartoes::pesquisarQuadro(const Identificador& cartao, Identificador* quadro) const {
    auto it = quadros.find(cartao.getValor());
    if (it == quadros.end()) return false;
    Identificador id;
    id.setValor(it->second);
    *quadro = id;
    return true;
}

bool ContainerCartoes::atualizar(const CartaoDeAtividade& cartao) {
    string chave = cartao.getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    // Preservar estado, entrada, inicio, termino e relacoes
    CartaoDeAtividade preservado = it->second;
    preservado.setNome(cartao.getNome());
    preservado.setDescricao(cartao.getDescricao());
    preservado.setPrioridade(cartao.getPrioridade());
    preservado.setTamanho(cartao.getTamanho());
    it->second = preservado;
    return true;
}

bool ContainerCartoes::atualizarCompleto(const CartaoDeAtividade& cartao) {
    string chave = cartao.getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    // Substitui o objeto inteiro; as relacoes ficam nos mapas separados e nao sao tocadas
    it->second = cartao;
    return true;
}

bool ContainerCartoes::remover(const Identificador& identificador) {
    string chave = identificador.getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    dados.erase(it);
    quadros.erase(chave);
    desenvolvedores.erase(chave);
    return true;
}

bool ContainerCartoes::existe(const Identificador& identificador) const {
    return dados.count(identificador.getValor()) > 0;
}

list<Identificador> ContainerCartoes::listar() const {
    list<Identificador> resultado;
    for (auto& par : dados) {
        resultado.push_back(par.second.getIdentificador());
    }
    return resultado;
}

list<CartaoDeAtividade> ContainerCartoes::listarPorQuadro(const Identificador& quadro) const {
    list<CartaoDeAtividade> resultado;
    for (auto& par : quadros) {
        if (par.second == quadro.getValor()) {
            auto it = dados.find(par.first);
            if (it != dados.end()) {
                resultado.push_back(it->second);
            }
        }
    }
    return resultado;
}

bool ContainerCartoes::quadroTemCartoes(const Identificador& quadro) const {
    for (auto& par : quadros) {
        if (par.second == quadro.getValor()) return true;
    }
    return false;
}

bool ContainerCartoes::desenvolvedorTemCartoes(const Email& email) const {
    for (auto& par : desenvolvedores) {
        if (par.second == email.getEmail()) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// CntrServicosCartaoDeAtividade
// ---------------------------------------------------------------------------

CntrServicosCartaoDeAtividade::CntrServicosCartaoDeAtividade(ContainerCartoes* cntrCartoes,
                                                              ContainerQuadros* cntrQuadros,
                                                              ContainerPessoas* cntrPessoas)
    : cntrCartoes(cntrCartoes), cntrQuadros(cntrQuadros), cntrPessoas(cntrPessoas) {}

bool CntrServicosCartaoDeAtividade::criar(const CartaoDeAtividade& cartao,
                                           const Identificador& quadro,
                                           const Email& desenvolvedor) {
    if (!cntrQuadros->existe(quadro)) return false;
    if (!cntrPessoas->existe(desenvolvedor)) return false;
    if (cartao.getEstado().getEstado() != "A FAZER") return false;
    return cntrCartoes->incluir(cartao, quadro, desenvolvedor);
}

bool CntrServicosCartaoDeAtividade::ler(CartaoDeAtividade* cartao) {
    return cntrCartoes->pesquisar(cartao);
}

bool CntrServicosCartaoDeAtividade::atualizar(const CartaoDeAtividade& cartao) {
    if (!cntrCartoes->existe(cartao.getIdentificador())) return false;
    return cntrCartoes->atualizar(cartao);
}

bool CntrServicosCartaoDeAtividade::excluir(const Identificador& identificador) {
    return cntrCartoes->remover(identificador);
}

bool CntrServicosCartaoDeAtividade::listar(list<Identificador>* lista) {
    *lista = cntrCartoes->listar();
    return true;
}

bool CntrServicosCartaoDeAtividade::mover(const Identificador& cartao,
                                           const Estado& destino,
                                           const Timestamp& momento) {
    CartaoDeAtividade c;
    c.setIdentificador(cartao);
    if (!cntrCartoes->pesquisar(&c)) return false;

    string estadoAtual   = c.getEstado().getEstado();
    string estadoDestino = destino.getEstado();

    if (estadoAtual == "A FAZER" && estadoDestino == "FAZENDO") {
        // Verificar WIP
        Identificador idQuadro;
        cntrCartoes->pesquisarQuadro(cartao, &idQuadro);
        Quadro q;
        q.setIdentificador(idQuadro);
        cntrQuadros->pesquisar(&q);

        int wip = 0;
        list<CartaoDeAtividade> lst = cntrCartoes->listarPorQuadro(idQuadro);
        for (auto& item : lst) {
            if (item.getEstado().getEstado() == "FAZENDO") wip++;
        }
        if (wip >= q.getLimite().getLimite()) return false;

        // momento deve ser >= entrada
        long minMomento = timestampParaMinutos(momento.getValor());
        long minEntrada = timestampParaMinutos(c.getEntrada().getValor());
        if (minMomento < minEntrada) return false;

        c.setEstado(destino);
        c.setInicio(momento);
        return cntrCartoes->atualizarCompleto(c);

    } else if (estadoAtual == "FAZENDO" && estadoDestino == "FEITO") {
        long minMomento = timestampParaMinutos(momento.getValor());
        long minInicio  = timestampParaMinutos(c.getInicio().getValor());
        if (minMomento < minInicio) return false;

        c.setEstado(destino);
        c.setTermino(momento);
        return cntrCartoes->atualizarCompleto(c);
    }

    return false;
}

bool CntrServicosCartaoDeAtividade::lerTempoCiclo(const Identificador& cartao, double& minutos) {
    CartaoDeAtividade c;
    c.setIdentificador(cartao);
    if (!cntrCartoes->pesquisar(&c)) return false;
    if (c.getEstado().getEstado() != "FEITO") return false;
    minutos = static_cast<double>(
        timestampParaMinutos(c.getTermino().getValor()) -
        timestampParaMinutos(c.getInicio().getValor()));
    return true;
}

bool CntrServicosCartaoDeAtividade::lerTempoCicloMedio(const Identificador& quadro, double& minutos) {
    if (!cntrQuadros->existe(quadro)) return false;
    list<CartaoDeAtividade> lst = cntrCartoes->listarPorQuadro(quadro);
    double soma = 0;
    int    n    = 0;
    for (auto& c : lst) {
        if (c.getEstado().getEstado() == "FEITO") {
            soma += static_cast<double>(
                timestampParaMinutos(c.getTermino().getValor()) -
                timestampParaMinutos(c.getInicio().getValor()));
            n++;
        }
    }
    if (n == 0) return false;
    minutos = soma / n;
    return true;
}

bool CntrServicosCartaoDeAtividade::lerLeadTime(const Identificador& cartao, double& minutos) {
    CartaoDeAtividade c;
    c.setIdentificador(cartao);
    if (!cntrCartoes->pesquisar(&c)) return false;
    if (c.getEstado().getEstado() != "FEITO") return false;
    minutos = static_cast<double>(
        timestampParaMinutos(c.getTermino().getValor()) -
        timestampParaMinutos(c.getEntrada().getValor()));
    return true;
}

bool CntrServicosCartaoDeAtividade::lerLeadTimeMedio(const Identificador& quadro, double& minutos) {
    if (!cntrQuadros->existe(quadro)) return false;
    list<CartaoDeAtividade> lst = cntrCartoes->listarPorQuadro(quadro);
    double soma = 0;
    int    n    = 0;
    for (auto& c : lst) {
        if (c.getEstado().getEstado() == "FEITO") {
            soma += static_cast<double>(
                timestampParaMinutos(c.getTermino().getValor()) -
                timestampParaMinutos(c.getEntrada().getValor()));
            n++;
        }
    }
    if (n == 0) return false;
    minutos = soma / n;
    return true;
}

bool CntrServicosCartaoDeAtividade::lerWip(const Identificador& quadro, int& wip) {
    if (!cntrQuadros->existe(quadro)) return false;
    wip = 0;
    list<CartaoDeAtividade> lst = cntrCartoes->listarPorQuadro(quadro);
    for (auto& c : lst) {
        if (c.getEstado().getEstado() == "FAZENDO") wip++;
    }
    return true;
}
