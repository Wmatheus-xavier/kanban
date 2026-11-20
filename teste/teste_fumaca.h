/**
 * @file teste_fumaca.h
 * @brief Declaracao da funcao de teste de fumaca do sistema Kanban
 *
 * Exercita as 26 funcionalidades do sistema em ordem de dependencia:
 * criacao de pessoas e autenticacao, projetos, quadros e cartoes;
 * movimentacao; metricas (WIP, tempo de ciclo, lead time e medias);
 * atualizacao; exclusao na ordem inversa.
 */

#ifndef KANBAN_TESTE_FUMACA_H
#define KANBAN_TESTE_FUMACA_H

/**
 * @brief Executa a bateria completa de 26 testes de fumaca
 *
 * Instancia containers e controladoras de servico diretamente (sem cin/cout
 * interativo). Imprime "[OK] N - descricao" ou "[FALHA] N - descricao" para
 * cada caso, seguido do resumo final.
 */
void executarTesteFumaca();

#endif // KANBAN_TESTE_FUMACA_H
