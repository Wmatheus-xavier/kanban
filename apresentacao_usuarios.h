/**
 * @file apresentacao_usuarios.h
 * @brief Declaracao das controladoras de apresentacao de autenticacao e de pessoas
 */

#ifndef KANBAN_APRESENTACAO_USUARIOS_H
#define KANBAN_APRESENTACAO_USUARIOS_H

#include "interfaces.h"

using namespace std;

// ---------------------------------------------------------------------------
// CntrIUAutenticacao
// ---------------------------------------------------------------------------

/**
 * @brief Controladora de interface de usuario para autenticacao
 *
 * Lê email e senha via cin, delega a verificacao para IServicosAutenticacao
 * e retorna o email e papel do usuario autenticado.
 */
class CntrIUAutenticacao : public IUAutenticacao {
  private:
    IServicosAutenticacao* cntrServicos = nullptr;
  public:
    bool autenticar(Email* email, Papel* papel) override;
    void setCntrServicosAutenticacao(IServicosAutenticacao* cntr) override;
};

// ---------------------------------------------------------------------------
// CntrIUPessoas
// ---------------------------------------------------------------------------

/**
 * @brief Controladora de interface de usuario para pessoas
 *
 * Exibe menus e le dados via cin/cout para as operacoes sobre pessoas.
 * O que e exibido depende do papel do usuario autenticado.
 */
class CntrIUPessoas : public IUPessoas {
  private:
    IServicosPessoas* cntrServicos = nullptr;

    bool criar(const Email& usuarioLogado);
    bool ler(const Email& usuarioLogado, const Papel& papel);
    bool atualizar(const Email& usuarioLogado, const Papel& papel);
    bool excluir(const Email& usuarioLogado, const Papel& papel);
    bool listar();
  public:
    bool cadastrar() override;
    bool executar(const Email& usuario, const Papel& papel) override;
    void setCntrServicosPessoas(IServicosPessoas* cntr) override;
};

#endif // KANBAN_APRESENTACAO_USUARIOS_H
