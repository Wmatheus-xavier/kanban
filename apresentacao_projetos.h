/**
 * @file apresentacao_projetos.h
 * @brief Declaracao da controladora de apresentacao de projetos
 */

#ifndef KANBAN_APRESENTACAO_PROJETOS_H
#define KANBAN_APRESENTACAO_PROJETOS_H

#include "interfaces.h"

using namespace std;

/**
 * @brief Controladora de interface de usuario para projetos
 *
 * Exibe menus e le dados via cin/cout para as operacoes sobre projetos.
 * Operacoes de criacao, atualizacao e exclusao sao restritas ao GESTOR.
 */
class CntrIUProjetos : public IUProjetos {
  private:
    IServicosProjetos* cntrServicos = nullptr;

    bool criar(const Email& gestor);
    bool ler();
    bool atualizar(const Email& gestor);
    bool excluir(const Email& gestor);
    bool listar();
  public:
    bool executar(const Email& email, const Papel& papel) override;
    void setCntrServicosProjetos(IServicosProjetos* cntr) override;
};

#endif // KANBAN_APRESENTACAO_PROJETOS_H
