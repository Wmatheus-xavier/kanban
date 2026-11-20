/**
 * @file servicos_projetos.h
 * @brief Declaracao do ContainerProjetos e CntrServicosProjetos
 */

#ifndef KANBAN_SERVICOS_PROJETOS_H
#define KANBAN_SERVICOS_PROJETOS_H

#include <map>
#include <list>
#include "interfaces/interfaces.h"

using namespace std;

// ===========================================================================
// ContainerProjetos
// ===========================================================================

/**
 * @brief Repositorio de projetos em memoria
 *
 * Armazena objetos Projeto indexados pelo identificador (string).
 * Mantém em mapa separado a relacao projeto -> email do gestor.
 * Nao aplica regras de negocio; e responsabilidade de CntrServicosProjetos.
 */
class ContainerProjetos {
  private:
    map<string, Projeto> dados;    ///< identificador.getValor() -> Projeto
    map<string, string>  gestores; ///< identificador.getValor() -> email do gestor
  public:
    /**
     * @brief Inclui um projeto associando-o a um gestor
     * @param projeto Objeto Projeto completamente preenchido
     * @param gestor  Email do gestor responsavel
     * @retval true  Incluido com sucesso
     * @retval false Identificador ja existe (chave duplicada)
     */
    bool incluir(const Projeto& projeto, const Email& gestor);

    /**
     * @brief Pesquisa um projeto pelo identificador
     *
     * Pre-condicao: projeto->getIdentificador() esta preenchido.
     * Pos-condicao (retval true): todos os campos de *projeto sao preenchidos.
     *
     * @param projeto Ponteiro que entra com o identificador e sai com todos os dados
     * @retval true  Encontrado; *projeto preenchido
     * @retval false Nao encontrado; *projeto inalterado
     */
    bool pesquisar(Projeto* projeto) const;

    /**
     * @brief Pesquisa o email do gestor de um projeto
     * @param identificador Identificador do projeto
     * @param gestor        Saida: email do gestor
     * @retval true  Gestor encontrado; *gestor preenchido
     * @retval false Projeto nao existe
     */
    bool pesquisarGestor(const Identificador& identificador, Email* gestor) const;

    /**
     * @brief Substitui os dados de um projeto existente; identificador nao muda
     * @param projeto Objeto com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Identificador nao encontrado
     */
    bool atualizar(const Projeto& projeto);

    /**
     * @brief Remove um projeto e sua relacao de gestor
     * @param identificador Identificador do projeto a remover
     * @retval true  Removido com sucesso
     * @retval false Identificador nao encontrado
     */
    bool remover(const Identificador& identificador);

    /**
     * @brief Verifica se existe um projeto com o identificador informado
     * @param identificador Identificador a verificar
     * @retval true  Existe
     * @retval false Nao existe
     */
    bool existe(const Identificador& identificador) const;

    /**
     * @brief Retorna a lista de identificadores de todos os projetos cadastrados
     * @return list<Identificador> em ordem alfanumerica
     */
    list<Identificador> listar() const;

    /**
     * @brief Verifica se o email informado e gestor de ao menos um projeto
     * @param email Email a verificar
     * @retval true  E gestor de ao menos um projeto
     * @retval false Nao e gestor de nenhum projeto
     */
    bool gestorTemProjetos(const Email& email) const;
};

// ===========================================================================
// CntrServicosProjetos
// ===========================================================================

/**
 * @brief Controladora de servico de projetos
 *
 * Implementa IServicosProjetos aplicando as seguintes regras de negocio:
 * - criar: o gestor deve existir em ContainerPessoas; identificador unico.
 * - atualizar: identificador nao e alterado; falha se projeto nao existe.
 * - excluir: falha se o projeto possuir ao menos um quadro vinculado.
 */
class CntrServicosProjetos : public IServicosProjetos {
  private:
    ContainerProjetos*       cntrProjetos;
    class ContainerPessoas*  cntrPessoas; ///< Usado para validar gestor em criar
    class ContainerQuadros*  cntrQuadros; ///< Usado para verificar exclusao
  public:
    /**
     * @brief Constroi a controladora injetando os tres containers necessarios
     * @param cntrProjetos Container de projetos
     * @param cntrPessoas  Container de pessoas (valida gestor em criar)
     * @param cntrQuadros  Container de quadros (verifica exclusao)
     */
    CntrServicosProjetos(ContainerProjetos* cntrProjetos,
                         ContainerPessoas*  cntrPessoas,
                         ContainerQuadros*  cntrQuadros);

    /**
     * @brief Cria um projeto associado ao gestor
     *
     * Falha se o gestor nao estiver cadastrado em ContainerPessoas
     * ou se o identificador ja existir.
     *
     * @param projeto Objeto completamente preenchido
     * @param gestor  Email do gestor (deve existir em ContainerPessoas)
     * @retval true  Criado com sucesso
     * @retval false Gestor inexistente ou identificador duplicado
     */
    bool criar(const Projeto& projeto, const Email& gestor) override;

    /**
     * @brief Le os dados de um projeto pelo identificador
     * @param projeto Ponteiro com identificador preenchido; sai com todos os dados
     * @retval true  Encontrado
     * @retval false Nao encontrado
     */
    bool ler(Projeto* projeto) override;

    /**
     * @brief Atualiza os dados de um projeto; identificador nao e alterado
     * @param projeto Objeto com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Projeto nao encontrado
     */
    bool atualizar(const Projeto& projeto) override;

    /**
     * @brief Exclui um projeto; falha se houver quadros vinculados
     * @param identificador Identificador do projeto a excluir
     * @retval true  Excluido com sucesso
     * @retval false Nao encontrado ou tem quadros vinculados
     */
    bool excluir(const Identificador& identificador) override;

    /**
     * @brief Lista os identificadores de todos os projetos cadastrados
     * @param lista Saida: list<Identificador> sobrescrita
     * @retval true  Sempre (mesmo que vazia)
     * @retval false Nao ocorre nesta implementacao
     */
    bool listar(list<Identificador>* lista) override;
};

#endif // KANBAN_SERVICOS_PROJETOS_H
