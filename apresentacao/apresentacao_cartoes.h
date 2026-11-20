/**
 * @file apresentacao_cartoes.h
 * @brief Declaracao da controladora de apresentacao de cartoes de atividade
 */

#ifndef KANBAN_APRESENTACAO_CARTOES_H
#define KANBAN_APRESENTACAO_CARTOES_H

#include "interfaces/interfaces.h"

using namespace std;

/**
 * @brief Controladora de interface de usuario para cartoes de atividade
 *
 * Exibe menus e le dados via cin/cout para as operacoes sobre cartoes.
 * Criacao, atualizacao e exclusao sao restritas ao GESTOR.
 * Leitura, listagem, movimentacao e metricas estao disponiveis para ambos os papeis.
 */
class CntrIUCartaoDeAtividade : public IUCartaoDeAtividade {
  private:
    IServicosCartaoDeAtividade* cntrServicos = nullptr;

    bool criar(const Email& gestor);
    bool ler();
    bool atualizar(const Email& gestor);
    bool excluir(const Email& gestor);
    bool listar();
    bool mover();
    bool lerTempoCiclo();
    bool lerTempoCicloMedio();
    bool lerLeadTime();
    bool lerLeadTimeMedio();
    bool lerWip();
  public:
    bool executar(const Email& email, const Papel& papel) override;
    void setCntrServicosCartaoDeAtividade(IServicosCartaoDeAtividade* cntr) override;
};

#endif // KANBAN_APRESENTACAO_CARTOES_H
