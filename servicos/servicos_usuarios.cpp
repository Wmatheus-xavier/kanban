/**
 * @file servicos_usuarios.cpp
 * @brief Implementacao de ContainerPessoas, CntrServicosAutenticacao e CntrServicosPessoas
 */

#include "servicos/servicos_usuarios.h"
#include "servicos/servicos_projetos.h"
#include "servicos/servicos_cartoes.h"

using namespace std;

// ===========================================================================
// ContainerPessoas
// ===========================================================================

bool ContainerPessoas::incluir(const Pessoa& pessoa) {
    string chave = pessoa.getEmail().getEmail();
    if (dados.count(chave)) return false;
    dados[chave] = pessoa;
    return true;
}

bool ContainerPessoas::pesquisar(Pessoa* pessoa) const {
    string chave = pessoa->getEmail().getEmail();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    *pessoa = it->second;
    return true;
}

bool ContainerPessoas::atualizar(const Pessoa& pessoa) {
    string chave = pessoa.getEmail().getEmail();
    auto it = dados.find(chave);
    if (it == dados.end()) return false;
    it->second = pessoa;  // substitui todo o objeto; email (chave) nao muda
    return true;
}

bool ContainerPessoas::remover(const Email& email) {
    auto it = dados.find(email.getEmail());
    if (it == dados.end()) return false;
    dados.erase(it);
    return true;
}

bool ContainerPessoas::existe(const Email& email) const {
    return dados.count(email.getEmail()) > 0;
}

list<Email> ContainerPessoas::listar() const {
    list<Email> resultado;
    for (const auto& par : dados) {
        resultado.push_back(par.second.getEmail());
    }
    return resultado;
}

// ===========================================================================
// CntrServicosAutenticacao
// ===========================================================================

CntrServicosAutenticacao::CntrServicosAutenticacao(ContainerPessoas* cp)
    : cntrPessoas(cp) {}

bool CntrServicosAutenticacao::autenticar(const Email& email,
                                           const Senha& senha,
                                           Papel* papel) {
    Pessoa p;
    p.setEmail(email);
    if (!cntrPessoas->pesquisar(&p)) return false;
    if (p.getSenha().getSenha() != senha.getSenha()) return false;
    *papel = p.getPapel();
    return true;
}

// ===========================================================================
// CntrServicosPessoas
// ===========================================================================

CntrServicosPessoas::CntrServicosPessoas(ContainerPessoas*  cntrPessoas,
                                          ContainerProjetos* cntrProjetos,
                                          ContainerCartoes*  cntrCartoes)
    : cntrPessoas(cntrPessoas), cntrProjetos(cntrProjetos), cntrCartoes(cntrCartoes) {}

bool CntrServicosPessoas::criar(const Pessoa& pessoa) {
    return cntrPessoas->incluir(pessoa);
}

bool CntrServicosPessoas::ler(Pessoa* pessoa) {
    return cntrPessoas->pesquisar(pessoa);
}

bool CntrServicosPessoas::atualizar(const Pessoa& pessoa) {
    // Verifica existencia antes de delegar (retorna false se nao existe)
    if (!cntrPessoas->existe(pessoa.getEmail())) return false;
    return cntrPessoas->atualizar(pessoa);
}

bool CntrServicosPessoas::excluir(const Email& email) {
    if (!cntrPessoas->existe(email)) return false;
    if (cntrProjetos->gestorTemProjetos(email)) return false;
    if (cntrCartoes->desenvolvedorTemCartoes(email)) return false;
    return cntrPessoas->remover(email);
}

bool CntrServicosPessoas::listar(list<Email>* lista) {
    *lista = cntrPessoas->listar();
    return true;
}
