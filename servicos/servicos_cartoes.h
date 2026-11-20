/**
 * @file servicos_cartoes.h
 * @brief Declaracao do ContainerCartoes e CntrServicosCartaoDeAtividade
 */

#ifndef KANBAN_SERVICOS_CARTOES_H
#define KANBAN_SERVICOS_CARTOES_H

#include <map>
#include <list>
#include "interfaces/interfaces.h"

using namespace std;

// ===========================================================================
// ContainerCartoes
// ===========================================================================

/**
 * @brief Repositorio de cartoes de atividade em memoria
 *
 * Armazena CartaoDeAtividade indexado pelo identificador (string).
 * Mantém em mapas separados as relacoes:
 * - cartao -> quadro  (pesquisarQuadro, listarPorQuadro, quadroTemCartoes)
 * - cartao -> email do desenvolvedor  (desenvolvedorTemCartoes)
 *
 * Nao aplica regras de negocio; e responsabilidade de CntrServicosCartaoDeAtividade.
 */
class ContainerCartoes {
  private:
    map<string, CartaoDeAtividade> dados;          ///< id cartao -> CartaoDeAtividade
    map<string, string>            quadros;         ///< id cartao -> id quadro
    map<string, string>            desenvolvedores; ///< id cartao -> email dev
  public:
    /**
     * @brief Inclui um cartao associando-o a um quadro e a um desenvolvedor
     * @param cartao        Objeto CartaoDeAtividade completamente preenchido
     * @param quadro        Identificador do quadro
     * @param desenvolvedor Email do desenvolvedor responsavel
     * @retval true  Incluido com sucesso
     * @retval false Identificador de cartao ja existe
     */
    bool incluir(const CartaoDeAtividade& cartao, const Identificador& quadro,
                 const Email& desenvolvedor);

    /**
     * @brief Pesquisa um cartao pelo identificador
     *
     * Pre-condicao: cartao->getIdentificador() esta preenchido.
     * Pos-condicao (retval true): todos os campos de *cartao sao preenchidos.
     *
     * @param cartao Ponteiro que entra com o identificador e sai com todos os dados
     * @retval true  Encontrado; *cartao preenchido
     * @retval false Nao encontrado
     */
    bool pesquisar(CartaoDeAtividade* cartao) const;

    /**
     * @brief Pesquisa o quadro ao qual um cartao pertence
     * @param cartao Identificador do cartao
     * @param quadro Saida: identificador do quadro
     * @retval true  Quadro encontrado; *quadro preenchido
     * @retval false Cartao nao existe
     */
    bool pesquisarQuadro(const Identificador& cartao, Identificador* quadro) const;

    /**
     * @brief Atualiza nome, descricao, prioridade e tamanho; preserva o restante
     *
     * Os campos estado, entrada, inicio, termino e as relacoes (quadro e
     * desenvolvedor) sao preservados independentemente do objeto fornecido.
     *
     * @param cartao Objeto com identificador existente; apenas nome, descricao,
     *               prioridade e tamanho sao usados
     * @retval true  Atualizado com sucesso
     * @retval false Identificador nao encontrado
     */
    bool atualizar(const CartaoDeAtividade& cartao);

    /**
     * @brief Substitui o objeto completo preservando as relacoes (quadro, desenvolvedor)
     *
     * Usado por CntrServicosCartaoDeAtividade::mover() para gravar estado e timestamps.
     * As entradas nos mapas de relacao nao sao alteradas.
     *
     * @param cartao CartaoDeAtividade completo a armazenar
     * @retval true  Atualizado com sucesso
     * @retval false Identificador nao encontrado
     */
    bool atualizarCompleto(const CartaoDeAtividade& cartao);

    /**
     * @brief Remove um cartao e suas relacoes de quadro e desenvolvedor
     * @param identificador Identificador do cartao a remover
     * @retval true  Removido com sucesso
     * @retval false Identificador nao encontrado
     */
    bool remover(const Identificador& identificador);

    /**
     * @brief Verifica se existe um cartao com o identificador informado
     * @param identificador Identificador a verificar
     * @retval true  Existe
     * @retval false Nao existe
     */
    bool existe(const Identificador& identificador) const;

    /**
     * @brief Retorna a lista de identificadores de todos os cartoes cadastrados
     * @return list<Identificador> em ordem alfanumerica
     */
    list<Identificador> listar() const;

    /**
     * @brief Retorna todos os cartoes pertencentes a um quadro
     * @param quadro Identificador do quadro
     * @return list<CartaoDeAtividade> dos cartoes do quadro
     */
    list<CartaoDeAtividade> listarPorQuadro(const Identificador& quadro) const;

    /**
     * @brief Verifica se um quadro tem ao menos um cartao vinculado
     * @param quadro Identificador do quadro
     * @retval true  Tem ao menos um cartao
     * @retval false Nao tem cartoes
     */
    bool quadroTemCartoes(const Identificador& quadro) const;

    /**
     * @brief Verifica se um desenvolvedor tem ao menos um cartao vinculado
     * @param email Email do desenvolvedor
     * @retval true  Tem ao menos um cartao
     * @retval false Nao tem cartoes
     */
    bool desenvolvedorTemCartoes(const Email& email) const;
};

// ===========================================================================
// CntrServicosCartaoDeAtividade
// ===========================================================================

/**
 * @brief Controladora de servico de cartao de atividade
 *
 * Implementa IServicosCartaoDeAtividade com todas as regras de negocio:
 *
 * criar: quadro e desenvolvedor devem existir; estado inicial obrigatorio "A FAZER".
 *
 * atualizar: altera apenas nome, descricao, prioridade e tamanho; preserva
 *            estado, entrada, inicio, termino e relacoes.
 *
 * mover (A FAZER -> FAZENDO):
 *   - Falha se WIP atual do quadro >= limite do quadro.
 *   - Falha se momento < entrada do cartao.
 *   - Grava inicio = momento.
 *
 * mover (FAZENDO -> FEITO):
 *   - Falha se momento < inicio do cartao.
 *   - Grava termino = momento.
 *
 * Metricas (em MINUTOS, calculadas com timestampParaMinutos()):
 *   - tempo de ciclo  = termino - inicio  (apenas para cartoes FEITO)
 *   - lead time       = termino - entrada (apenas para cartoes FEITO)
 *   - WIP             = numero de cartoes do quadro em FAZENDO
 */
class CntrServicosCartaoDeAtividade : public IServicosCartaoDeAtividade {
  private:
    ContainerCartoes*        cntrCartoes;
    class ContainerQuadros*  cntrQuadros; ///< Valida quadro; calcula WIP; verifica limite
    class ContainerPessoas*  cntrPessoas; ///< Valida desenvolvedor em criar
  public:
    /**
     * @brief Constroi a controladora injetando os tres containers necessarios
     * @param cntrCartoes  Container de cartoes
     * @param cntrQuadros  Container de quadros (WIP, limite, validacao de quadro)
     * @param cntrPessoas  Container de pessoas (valida desenvolvedor)
     */
    CntrServicosCartaoDeAtividade(ContainerCartoes* cntrCartoes,
                                  ContainerQuadros* cntrQuadros,
                                  ContainerPessoas* cntrPessoas);

    /**
     * @brief Cria um cartao; exige quadro e desenvolvedor existentes e estado A FAZER
     * @param cartao        Objeto com estado "A FAZER" e entrada preenchida
     * @param quadro        Identificador do quadro (deve existir)
     * @param desenvolvedor Email do desenvolvedor (deve existir em ContainerPessoas)
     * @retval true  Criado com sucesso
     * @retval false Identificador duplicado, quadro inexistente, desenvolvedor
     *               inexistente ou estado diferente de A FAZER
     */
    bool criar(const CartaoDeAtividade& cartao, const Identificador& quadro,
               const Email& desenvolvedor) override;

    /**
     * @brief Le os dados de um cartao pelo identificador
     * @param cartao Ponteiro com identificador preenchido; sai com todos os dados
     * @retval true  Encontrado
     * @retval false Nao encontrado
     */
    bool ler(CartaoDeAtividade* cartao) override;

    /**
     * @brief Atualiza nome, descricao, prioridade e tamanho; preserva o restante
     * @param cartao Objeto com identificador existente; outros campos usados sao
     *               apenas nome, descricao, prioridade e tamanho
     * @retval true  Atualizado com sucesso
     * @retval false Nao encontrado
     */
    bool atualizar(const CartaoDeAtividade& cartao) override;

    /**
     * @brief Exclui um cartao
     * @param identificador Identificador do cartao a excluir
     * @retval true  Excluido com sucesso
     * @retval false Nao encontrado
     */
    bool excluir(const Identificador& identificador) override;

    /**
     * @brief Lista os identificadores de todos os cartoes cadastrados
     * @param lista Saida: list<Identificador> sobrescrita
     * @retval true  Sempre
     * @retval false Nao ocorre nesta implementacao
     */
    bool listar(list<Identificador>* lista) override;

    /**
     * @brief Move um cartao para o proximo estado, gravando o timestamp correto
     *
     * Transicoes validas:
     * - A FAZER  -> FAZENDO: grava inicio=momento.
     *   Falha se WIP do quadro >= limite ou momento < entrada.
     * - FAZENDO  -> FEITO:   grava termino=momento.
     *   Falha se momento < inicio.
     *
     * @param cartao  Identificador do cartao
     * @param destino Estado destino (FAZENDO ou FEITO)
     * @param momento Timestamp do instante da movimentacao
     * @retval true  Movido; timestamp gravado
     * @retval false Cartao inexistente, transicao invalida, WIP excedido
     *               ou momento anterior ao ultimo timestamp do cartao
     */
    bool mover(const Identificador& cartao, const Estado& destino,
               const Timestamp& momento) override;

    /**
     * @brief Calcula o tempo de ciclo de um cartao em minutos
     *
     * Tempo de ciclo (minutos) = termino - inicio, usando timestampParaMinutos().
     * Exige estado FEITO.
     *
     * @param[in]  cartao  Identificador do cartao
     * @param[out] minutos Tempo de ciclo em minutos (double)
     * @retval true  Calculado com sucesso
     * @retval false Cartao nao encontrado ou nao esta em FEITO
     */
    bool lerTempoCiclo(const Identificador& cartao, double& minutos) override;

    /**
     * @brief Calcula o tempo de ciclo medio dos cartoes FEITO de um quadro em minutos
     *
     * Media aritmetica dos tempos de ciclo (minutos) de todos os cartoes
     * do quadro que estejam em estado FEITO.
     *
     * @param[in]  quadro  Identificador do quadro
     * @param[out] minutos Media dos tempos de ciclo em minutos (double)
     * @retval true  Calculado (ao menos um cartao FEITO no quadro)
     * @retval false Quadro nao encontrado ou nenhum cartao em FEITO
     */
    bool lerTempoCicloMedio(const Identificador& quadro, double& minutos) override;

    /**
     * @brief Calcula o lead time de um cartao em minutos
     *
     * Lead time (minutos) = termino - entrada, usando timestampParaMinutos().
     * Exige estado FEITO.
     *
     * @param[in]  cartao  Identificador do cartao
     * @param[out] minutos Lead time em minutos (double)
     * @retval true  Calculado com sucesso
     * @retval false Cartao nao encontrado ou nao esta em FEITO
     */
    bool lerLeadTime(const Identificador& cartao, double& minutos) override;

    /**
     * @brief Calcula o lead time medio dos cartoes FEITO de um quadro em minutos
     *
     * Media aritmetica dos lead times (minutos) de todos os cartoes
     * do quadro que estejam em estado FEITO.
     *
     * @param[in]  quadro  Identificador do quadro
     * @param[out] minutos Media dos lead times em minutos (double)
     * @retval true  Calculado (ao menos um cartao FEITO no quadro)
     * @retval false Quadro nao encontrado ou nenhum cartao em FEITO
     */
    bool lerLeadTimeMedio(const Identificador& quadro, double& minutos) override;

    /**
     * @brief Le o WIP (work in progress) atual de um quadro
     *
     * WIP = numero de cartoes do quadro em estado "FAZENDO".
     *
     * @param[in]  quadro Identificador do quadro
     * @param[out] wip    Numero de cartoes em FAZENDO (inteiro >= 0)
     * @retval true  Leitura realizada
     * @retval false Quadro nao encontrado
     */
    bool lerWip(const Identificador& quadro, int& wip) override;
};

#endif // KANBAN_SERVICOS_CARTOES_H
