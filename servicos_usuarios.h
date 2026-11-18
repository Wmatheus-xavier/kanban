/**
 * @file servicos_usuarios.h
 * @brief Declaracao do ContainerPessoas, CntrServicosAutenticacao e CntrServicosPessoas
 */

#ifndef KANBAN_SERVICOS_USUARIOS_H
#define KANBAN_SERVICOS_USUARIOS_H

#include <map>
#include <list>
#include "interfaces.h"

using namespace std;

// ===========================================================================
// ContainerPessoas
// ===========================================================================

/**
 * @brief Repositorio de pessoas em memoria
 *
 * Armazena objetos Pessoa indexados pelo email (string).
 * Nao aplica regras de negocio; e responsabilidade de CntrServicosPessoas.
 */
class ContainerPessoas {
  private:
    map<string, Pessoa> dados; ///< email.getEmail() -> Pessoa
  public:
    /**
     * @brief Inclui uma nova pessoa no repositorio
     * @param pessoa Objeto Pessoa completamente preenchido
     * @retval true  Incluida com sucesso
     * @retval false Email ja existe (chave duplicada)
     */
    bool incluir(const Pessoa& pessoa);

    /**
     * @brief Pesquisa uma pessoa pelo email
     *
     * Pre-condicao: pessoa->getEmail() esta preenchido com o email buscado.
     * Pos-condicao (retval true): todos os campos de *pessoa sao preenchidos.
     *
     * @param pessoa Ponteiro que entra com o email e sai com todos os dados
     * @retval true  Encontrada; *pessoa preenchida
     * @retval false Nao encontrada; *pessoa inalterado
     */
    bool pesquisar(Pessoa* pessoa) const;

    /**
     * @brief Substitui os dados de uma pessoa existente pelo email
     *
     * O email (chave) permanece inalterado.
     *
     * @param pessoa Objeto com email existente e novos dados
     * @retval true  Atualizada com sucesso
     * @retval false Email nao encontrado
     */
    bool atualizar(const Pessoa& pessoa);

    /**
     * @brief Remove uma pessoa pelo email
     * @param email Email da pessoa a remover
     * @retval true  Removida com sucesso
     * @retval false Email nao encontrado
     */
    bool remover(const Email& email);

    /**
     * @brief Verifica se existe uma pessoa com o email informado
     * @param email Email a verificar
     * @retval true  Existe
     * @retval false Nao existe
     */
    bool existe(const Email& email) const;

    /**
     * @brief Retorna a lista de emails de todas as pessoas cadastradas
     * @return list<Email> em ordem de insercao do map (alfanumerica por email)
     */
    list<Email> listar() const;
};

// ===========================================================================
// CntrServicosAutenticacao
// ===========================================================================

/**
 * @brief Controladora de servico de autenticacao
 *
 * Implementa IServicosAutenticacao consultando o ContainerPessoas.
 * Retorna true apenas se o email existe e a senha confere (comparacao direta
 * de string entre getSenha() do objeto armazenado e o parametro fornecido).
 */
class CntrServicosAutenticacao : public IServicosAutenticacao {
  private:
    ContainerPessoas* cntrPessoas; ///< Container consultado na autenticacao
  public:
    /**
     * @brief Constroi a controladora injetando o container de pessoas
     * @param cntrPessoas Ponteiro valido para ContainerPessoas (nao nulo)
     */
    explicit CntrServicosAutenticacao(ContainerPessoas* cntrPessoas);

    /**
     * @brief Autentica email e senha; em caso de sucesso preenche *papel
     * @param email Email do usuario
     * @param senha Senha fornecida
     * @param papel Saida: papel do usuario autenticado
     * @retval true  Email existe e senha confere; *papel preenchido
     * @retval false Email nao cadastrado ou senha incorreta; *papel indefinido
     */
    bool autenticar(const Email& email, const Senha& senha, Papel* papel) override;
};

// ===========================================================================
// CntrServicosPessoas
// ===========================================================================

/**
 * @brief Controladora de servico de pessoas
 *
 * Implementa IServicosPessoas aplicando as seguintes regras de negocio:
 * - criar: falha se o email ja estiver cadastrado.
 * - atualizar: nao altera o email (chave primaria); falha se pessoa nao existe.
 * - excluir: falha se a pessoa for gestor de ao menos um projeto, ou
 *            desenvolvedor de ao menos um cartao de atividade.
 */
class CntrServicosPessoas : public IServicosPessoas {
  private:
    ContainerPessoas*        cntrPessoas;
    class ContainerProjetos* cntrProjetos; ///< Usado para verificar se e gestor
    class ContainerCartoes*  cntrCartoes;  ///< Usado para verificar se e desenvolvedor
  public:
    /**
     * @brief Constroi a controladora injetando os tres containers necessarios
     * @param cntrPessoas  Container de pessoas
     * @param cntrProjetos Container de projetos (verificacao de exclusao)
     * @param cntrCartoes  Container de cartoes  (verificacao de exclusao)
     */
    CntrServicosPessoas(ContainerPessoas*  cntrPessoas,
                        ContainerProjetos* cntrProjetos,
                        ContainerCartoes*  cntrCartoes);

    /**
     * @brief Cadastra uma nova pessoa
     * @param pessoa Objeto completamente preenchido
     * @retval true  Criada com sucesso
     * @retval false Email ja existe
     */
    bool criar(const Pessoa& pessoa) override;

    /**
     * @brief Le os dados de uma pessoa pelo email
     * @param pessoa Ponteiro com email preenchido; sai com todos os dados
     * @retval true  Encontrada
     * @retval false Nao encontrada
     */
    bool ler(Pessoa* pessoa) override;

    /**
     * @brief Atualiza nome, senha e papel de uma pessoa; email nao e alterado
     * @param pessoa Objeto com email existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Pessoa nao encontrada
     */
    bool atualizar(const Pessoa& pessoa) override;

    /**
     * @brief Exclui uma pessoa; falha se ela tiver vinculos ativos
     *
     * Condicoes de falha (alem de nao existir):
     * - e gestor de ao menos um projeto (ContainerProjetos::gestorTemProjetos)
     * - e desenvolvedor de ao menos um cartao (ContainerCartoes::desenvolvedorTemCartoes)
     *
     * @param email Email da pessoa a excluir
     * @retval true  Excluida
     * @retval false Nao encontrada ou tem vinculos ativos
     */
    bool excluir(const Email& email) override;

    /**
     * @brief Lista os emails de todas as pessoas cadastradas
     * @param lista Saida: list<Email> sobrescrita com todos os emails
     * @retval true  Sempre (mesmo que a lista fique vazia)
     * @retval false Nao ocorre nesta implementacao
     */
    bool listar(list<Email>* lista) override;
};

#endif // KANBAN_SERVICOS_USUARIOS_H
