/**
 * @file servicos_projetos.cpp
 * @brief Implementacao de ContainerProjetos e CntrServicosProjetos
 */

#include "servicos/servicos_projetos.h"
#include "servicos/servicos_usuarios.h"
#include "servicos/servicos_quadros.h"

using namespace std;

// ---------------------------------------------------------------------------
// ContainerProjetos
// ---------------------------------------------------------------------------

bool ContainerProjetos::incluir(const Projeto& projeto, const Email& gestor) {
    string chave = projeto.getIdentificador().getValor();
    if (dados.count(chave)) return false;
    dados[chave]   = projeto;
    gestores[chave] = gestor.getEmail();
    return true;
}

bool ContainerProjetos::pesquisar(Projeto* projeto) const {
    string chave = projeto->getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    *projeto = it->second;
    return true;
}

bool ContainerProjetos::pesquisarGestor(const Identificador& identificador, Email* gestor) const {
    auto it = gestores.find(identificador.getValor());
    if (it == gestores.end()) return false;
    Email e;
    e.setEmail(it->second);
    *gestor = e;
    return true;
}

bool ContainerProjetos::atualizar(const Projeto& projeto) {
    string chave = projeto.getIdentificador().getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    it->second = projeto;
    return true;
}

bool ContainerProjetos::remover(const Identificador& identificador) {
    string chave = identificador.getValor();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    dados.erase(it);
    gestores.erase(chave);
    return true;
}

bool ContainerProjetos::existe(const Identificador& identificador) const {
    return dados.count(identificador.getValor()) > 0;
}

list<Identificador> ContainerProjetos::listar() const {
    list<Identificador> resultado;
    for (auto& par : dados) {
        resultado.push_back(par.second.getIdentificador());
    }
    return resultado;
}

bool ContainerProjetos::gestorTemProjetos(const Email& email) const {
    for (auto& par : gestores) {
        if (par.second == email.getEmail()) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// CntrServicosProjetos
// ---------------------------------------------------------------------------

CntrServicosProjetos::CntrServicosProjetos(ContainerProjetos* cntrProjetos,
                                            ContainerPessoas*  cntrPessoas,
                                            ContainerQuadros*  cntrQuadros)
    : cntrProjetos(cntrProjetos), cntrPessoas(cntrPessoas), cntrQuadros(cntrQuadros) {}

bool CntrServicosProjetos::criar(const Projeto& projeto, const Email& gestor) {
    // Gestor deve existir
    if (!cntrPessoas->existe(gestor)) return false;
    return cntrProjetos->incluir(projeto, gestor);
}

bool CntrServicosProjetos::ler(Projeto* projeto) {
    return cntrProjetos->pesquisar(projeto);
}

bool CntrServicosProjetos::atualizar(const Projeto& projeto) {
    if (!cntrProjetos->existe(projeto.getIdentificador())) return false;
    return cntrProjetos->atualizar(projeto);
}

bool CntrServicosProjetos::excluir(const Identificador& identificador) {
    if (!cntrProjetos->existe(identificador)) return false;
    // Nao pode excluir se tiver quadros
    if (cntrQuadros->projetoTemQuadros(identificador)) return false;
    return cntrProjetos->remover(identificador);
}

bool CntrServicosProjetos::listar(list<Identificador>* lista) {
    *lista = cntrProjetos->listar();
    return true;
}
