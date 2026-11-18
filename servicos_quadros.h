/**
 * @file servicos_quadros.h
 * @brief Declaracao do ContainerQuadros e CntrServicosQuadros
 */

#ifndef KANBAN_SERVICOS_QUADROS_H
#define KANBAN_SERVICOS_QUADROS_H

#include <map>
#include <list>
#include "interfaces.h"

using namespace std;

// ===========================================================================
// ContainerQuadros
// ===========================================================================

/**
 * @brief Repositorio de quadros em memoria
 *
 * Armazena objetos Quadro indexados pelo identificador (string).
 * Mantém em mapa separado a relacao quadro -> identificador do projeto.
 * Nao aplica regras de negocio; e responsabilidade de CntrServicosQuadros.
 */
class ContainerQuadros {
  private:
    map<string, Quadro>  dados;    ///< identificador.getValor() -> Quadro
    map<string, string>  projetos; ///< identificador do quadro -> identificador do projeto
  public:
    /**
     * @brief Inclui um quadro associando-o a um projeto
     * @param quadro  Objeto Quadro completamente preenchido
     * @param projeto Identificador do projeto ao qual o quadro pertence
     * @retval true  Incluido com sucesso
     * @retval false Identificador de quadro ja existe (chave duplicada)
     */
    bool incluir(const Quadro& quadro, const Identificador& projeto);

    /**
     * @brief Pesquisa um quadro pelo identificador
     *
     * Pre-condicao: quadro->getIdentificador() esta preenchido.
     * Pos-condicao (retval true): todos os campos de *quadro sao preenchidos.
     *
     * @param quadro Ponteiro que entra com o identificador e sai com todos os dados
     * @retval true  Encontrado; *quadro preenchido
     * @retval false Nao encontrado
     */
    bool pesquisar(Quadro* quadro) const;

    /**
     * @brief Pesquisa o identificador do projeto ao qual um quadro pertence
     * @param quadro  Identificador do quadro
     * @param projeto Saida: identificador do projeto
     * @retval true  Projeto encontrado; *projeto preenchido
     * @retval false Quadro nao existe
     */
    bool pesquisarProjeto(const Identificador& quadro, Identificador* projeto) const;

    /**
     * @brief Substitui os dados de um quadro existente; identificador nao muda
     * @param quadro Objeto com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Identificador nao encontrado
     */
    bool atualizar(const Quadro& quadro);

    /**
     * @brief Remove um quadro e sua relacao de projeto
     * @param identificador Identificador do quadro a remover
     * @retval true  Removido com sucesso
     * @retval false Identificador nao encontrado
     */
    bool remover(const Identificador& identificador);

    /**
     * @brief Verifica se existe um quadro com o identificador informado
     * @param identificador Identificador a verificar
     * @retval true  Existe
     * @retval false Nao existe
     */
    bool existe(const Identificador& identificador) const;

    /**
     * @brief Retorna a lista de identificadores de todos os quadros cadastrados
     * @return list<Identificador> em ordem alfanumerica
     */
    list<Identificador> listar() const;

    /**
     * @brief Verifica se um projeto tem ao menos um quadro vinculado
     * @param projeto Identificador do projeto
     * @retval true  Tem ao menos um quadro
     * @retval false Nao tem quadros
     */
    bool projetoTemQuadros(const Identificador& projeto) const;
};

// ===========================================================================
// CntrServicosQuadros
// ===========================================================================

/**
 * @brief Controladora de servico de quadros
 *
 * Implementa IServicosQuadros aplicando as seguintes regras de negocio:
 * - criar: o projeto deve existir em ContainerProjetos; identificador unico.
 * - atualizar: falha se novo limite < WIP atual do quadro (numero de cartoes
 *              em estado FAZENDO); identificador nao e alterado.
 * - excluir: falha se o quadro possuir ao menos um cartao vinculado.
 */
class CntrServicosQuadros : public IServicosQuadros {
  private:
    ContainerQuadros*        cntrQuadros;
    class ContainerProjetos* cntrProjetos; ///< Valida projeto em criar
    class ContainerCartoes*  cntrCartoes;  ///< Calcula WIP em atualizar; verifica exclusao
  public:
    /**
     * @brief Constroi a controladora injetando os tres containers necessarios
     * @param cntrQuadros  Container de quadros
     * @param cntrProjetos Container de projetos (valida projeto em criar)
     * @param cntrCartoes  Container de cartoes (WIP em atualizar; exclusao)
     */
    CntrServicosQuadros(ContainerQuadros*  cntrQuadros,
                        ContainerProjetos* cntrProjetos,
                        ContainerCartoes*  cntrCartoes);

    /**
     * @brief Cria um quadro vinculado a um projeto existente
     *
     * @param quadro  Objeto completamente preenchido
     * @param projeto Identificador do projeto (deve existir em ContainerProjetos)
     * @retval true  Criado com sucesso
     * @retval false Identificador de quadro duplicado ou projeto inexistente
     */
    bool criar(const Quadro& quadro, const Identificador& projeto) override;

    /**
     * @brief Le os dados de um quadro pelo identificador
     * @param quadro Ponteiro com identificador preenchido; sai com todos os dados
     * @retval true  Encontrado
     * @retval false Nao encontrado
     */
    bool ler(Quadro* quadro) override;

    /**
     * @brief Atualiza nome e limite do quadro; falha se novo limite < WIP atual
     *
     * WIP atual = numero de cartoes do quadro com estado "FAZENDO".
     * O identificador nao e alterado.
     *
     * @param quadro Objeto com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Nao encontrado ou quadro.getLimite() < WIP atual
     */
    bool atualizar(const Quadro& quadro) override;

    /**
     * @brief Exclui um quadro; falha se houver cartoes vinculados
     * @param identificador Identificador do quadro a excluir
     * @retval true  Excluido com sucesso
     * @retval false Nao encontrado ou tem cartoes vinculados
     */
    bool excluir(const Identificador& identificador) override;

    /**
     * @brief Lista os identificadores de todos os quadros cadastrados
     * @param lista Saida: list<Identificador> sobrescrita
     * @retval true  Sempre (mesmo que vazia)
     * @retval false Nao ocorre nesta implementacao
     */
    bool listar(list<Identificador>* lista) override;
};

#endif // KANBAN_SERVICOS_QUADROS_H
