/**
 * @file apresentacao_quadros.h
 * @brief Declaracao da controladora de apresentacao de quadros
 */

#ifndef KANBAN_APRESENTACAO_QUADROS_H
#define KANBAN_APRESENTACAO_QUADROS_H

#include "interfaces.h"

using namespace std;

/**
 * @brief Controladora de interface de usuario para quadros
 *
 * Exibe menus e le dados via cin/cout para as operacoes sobre quadros.
 * Operacoes de criacao, atualizacao e exclusao sao restritas ao GESTOR.
 */
class CntrIUQuadros : public IUQuadros {
  private:
    IServicosQuadros* cntrServicos = nullptr;

    bool criar(const Email& gestor);
    bool ler();
    bool atualizar(const Email& gestor);
    bool excluir(const Email& gestor);
    bool listar();
  public:
    bool executar(const Email& email, const Papel& papel) override;
    void setCntrServicosQuadros(IServicosQuadros* cntr) override;
};

#endif // KANBAN_APRESENTACAO_QUADROS_H
