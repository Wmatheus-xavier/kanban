/**
 * @file servicos_quadros.cpp
 * @brief Implementacao de ContainerQuadros e CntrServicosQuadros
 */

#include "servicos_quadros.h"
#include "servicos_projetos.h"
#include "servicos_cartoes.h"

using namespace std;

// ---------------------------------------------------------------------------
// ContainerQuadros
// ---------------------------------------------------------------------------

bool ContainerQuadros::incluir(const Quadro& quadro, const Identificador& projeto) {
    string chave = quadro.getIdentificador().getValor();
    if (dados.count(chave)) return false;
    dados[chave]    = quadro;
    projetos[chave] = projeto.getValor();
    return true;
}

bool ContainerQuadros::pesquisar(Quadro* quadro) const {
    string chave = quadro->getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    *quadro = it->second;
    return true;
}

bool ContainerQuadros::pesquisarProjeto(const Identificador& quadro, Identificador* projeto) const {
    auto it = projetos.find(quadro.getValor());
    if (it == projetos.end()) return false;
    Identificador id;
    id.setValor(it->second);
    *projeto = id;
    return true;
}

bool ContainerQuadros::atualizar(const Quadro& quadro) {
    string chave = quadro.getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    it->second = quadro;
    return true;
}

bool ContainerQuadros::remover(const Identificador& identificador) {
    string chave = identificador.getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    dados.erase(it);
    projetos.erase(chave);
    return true;
}

bool ContainerQuadros::existe(const Identificador& identificador) const {
    return dados.count(identificador.getValor()) > 0;
}

list<Identificador> ContainerQuadros::listar() const {
    list<Identificador> resultado;
    for (auto& par : dados) {
        resultado.push_back(par.second.getIdentificador());
    }
    return resultado;
}

bool ContainerQuadros::projetoTemQuadros(const Identificador& projeto) const {
    for (auto& par : projetos) {
        if (par.second == projeto.getValor()) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// CntrServicosQuadros
// ---------------------------------------------------------------------------

CntrServicosQuadros::CntrServicosQuadros(ContainerQuadros*  cntrQuadros,
                                          ContainerProjetos* cntrProjetos,
                                          ContainerCartoes*  cntrCartoes)
    : cntrQuadros(cntrQuadros), cntrProjetos(cntrProjetos), cntrCartoes(cntrCartoes) {}

bool CntrServicosQuadros::criar(const Quadro& quadro, const Identificador& projeto) {
    if (!cntrProjetos->existe(projeto)) return false;
    return cntrQuadros->incluir(quadro, projeto);
}

bool CntrServicosQuadros::ler(Quadro* quadro) {
    return cntrQuadros->pesquisar(quadro);
}

bool CntrServicosQuadros::atualizar(const Quadro& quadro) {
    if (!cntrQuadros->existe(quadro.getIdentificador())) return false;
    // Verificar se novo limite >= WIP atual
    int wip = 0;
    list<CartaoDeAtividade> cartoes = cntrCartoes->listarPorQuadro(quadro.getIdentificador());
    for (auto& c : cartoes) {
        if (c.getEstado().getEstado() == "FAZENDO") wip++;
    }
    if (quadro.getLimite().getLimite() < wip) return false;
    return cntrQuadros->atualizar(quadro);
}

bool CntrServicosQuadros::excluir(const Identificador& identificador) {
    if (!cntrQuadros->existe(identificador)) return false;
    if (cntrCartoes->quadroTemCartoes(identificador)) return false;
    return cntrQuadros->remover(identificador);
}

bool CntrServicosQuadros::listar(list<Identificador>* lista) {
    *lista = cntrQuadros->listar();
    return true;
}
