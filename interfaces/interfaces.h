/**
 * @file interfaces.h
 * @brief Declaracao das interfaces que conectam as camadas de servico e apresentacao
 */

#ifndef KANBAN_INTERFACES_H
#define KANBAN_INTERFACES_H

#include <list>
#include "dominio/dominios.h"
#include "entidades/entidades.h"

using namespace std;

// ---------------------------------------------------------------------------
// Autenticacao
// ---------------------------------------------------------------------------

/**
 * @brief Interface dos servicos de autenticacao
 *
 * Permite verificar as credenciais de um usuario e obter o seu papel.
 */
class IServicosAutenticacao {
  public:
    virtual ~IServicosAutenticacao() = default;

    /**
     * @brief Autentica um usuario pelo email e senha
     * @param email  Email do usuario
     * @param senha  Senha do usuario
     * @param papel  Ponteiro que receberá o papel do usuario autenticado
     * @retval true  Credenciais validas
     * @retval false Credenciais invalidas
     */
    virtual bool autenticar(const Email& email, const Senha& senha, Papel* papel) = 0;
};

/**
 * @brief Interface da apresentacao de autenticacao
 *
 * Coordena a interacao com o usuario para a autenticacao no sistema.
 */
class IUAutenticacao {
  public:
    virtual ~IUAutenticacao() = default;

    /**
     * @brief Executa o fluxo de autenticacao
     * @param email Ponteiro que receberá o email do usuario autenticado
     * @param papel Ponteiro que receberá o papel do usuario autenticado
     * @retval true  Autenticacao realizada com sucesso
     * @retval false Autenticacao falhou ou usuario cancelou
     */
    virtual bool autenticar(Email* email, Papel* papel) = 0;

    /**
     * @brief Define a controladora de servicos de autenticacao
     * @param cntr Ponteiro para a controladora que implementa IServicosAutenticacao
     */
    virtual void setCntrServicosAutenticacao(IServicosAutenticacao* cntr) = 0;
};

// ---------------------------------------------------------------------------
// Pessoas
// ---------------------------------------------------------------------------

/**
 * @brief Interface dos servicos de pessoa
 *
 * Oferece as operacoes CRUD e listagem sobre pessoas cadastradas.
 */
class IServicosPessoas {
  public:
    virtual ~IServicosPessoas() = default;

    /**
     * @brief Cadastra uma nova pessoa
     * @param pessoa Pessoa a ser cadastrada (email como chave)
     * @retval true  Pessoa criada
     * @retval false Email ja cadastrado
     */
    virtual bool criar(const Pessoa& pessoa) = 0;

    /**
     * @brief Le os dados de uma pessoa
     * @param pessoa Ponteiro que entra com o email e sai com os dados preenchidos
     * @retval true  Pessoa encontrada
     * @retval false Pessoa nao encontrada
     */
    virtual bool ler(Pessoa* pessoa) = 0;

    /**
     * @brief Atualiza os dados de uma pessoa
     * @param pessoa Pessoa com email existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Pessoa nao encontrada ou dados invalidos
     */
    virtual bool atualizar(const Pessoa& pessoa) = 0;

    /**
     * @brief Exclui uma pessoa
     * @param email Email da pessoa a excluir
     * @retval true  Excluida com sucesso
     * @retval false Pessoa nao encontrada ou exclusao causaria inconsistencia
     */
    virtual bool excluir(const Email& email) = 0;

    /**
     * @brief Lista os emails de todas as pessoas cadastradas
     * @param lista Ponteiro para a lista que sera preenchida
     * @retval true  Listagem realizada
     * @retval false Falha na listagem
     */
    virtual bool listar(list<Email>* lista) = 0;
};

/**
 * @brief Interface da apresentacao de pessoas
 *
 * Coordena a interacao com o usuario para operacoes sobre pessoas.
 */
class IUPessoas {
  public:
    virtual ~IUPessoas() = default;

    /**
     * @brief Cria uma nova conta sem autenticacao previa
     * @retval true  Conta criada com sucesso
     * @retval false Criacao cancelada ou falhou
     */
    virtual bool cadastrar() = 0;

    /**
     * @brief Executa o menu de operacoes sobre pessoas
     * @param usuario Email do usuario autenticado
     * @param papel   Papel do usuario autenticado
     * @retval true  Operacao concluida
     * @retval false Falha
     */
    virtual bool executar(const Email& usuario, const Papel& papel) = 0;

    /**
     * @brief Define a controladora de servicos de pessoas
     * @param cntr Ponteiro para a controladora que implementa IServicosPessoas
     */
    virtual void setCntrServicosPessoas(IServicosPessoas* cntr) = 0;
};

// ---------------------------------------------------------------------------
// Projetos
// ---------------------------------------------------------------------------

/**
 * @brief Interface dos servicos de projeto
 *
 * Oferece as operacoes CRUD e listagem sobre projetos.
 */
class IServicosProjetos {
  public:
    virtual ~IServicosProjetos() = default;

    /**
     * @brief Cria um novo projeto
     * @param projeto Projeto a ser criado
     * @param gestor  Email do gestor responsavel pelo projeto
     * @retval true  Projeto criado
     * @retval false Identificador ja existente ou gestor inexistente
     */
    virtual bool criar(const Projeto& projeto, const Email& gestor) = 0;

    /**
     * @brief Le os dados de um projeto
     * @param projeto Ponteiro que entra com o identificador e sai com os dados preenchidos
     * @retval true  Projeto encontrado
     * @retval false Projeto nao encontrado
     */
    virtual bool ler(Projeto* projeto) = 0;

    /**
     * @brief Atualiza os dados de um projeto
     * @param projeto Projeto com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Projeto nao encontrado ou dados invalidos
     */
    virtual bool atualizar(const Projeto& projeto) = 0;

    /**
     * @brief Exclui um projeto
     * @param identificador Identificador do projeto a excluir
     * @retval true  Excluido com sucesso
     * @retval false Projeto nao encontrado ou possui quadros vinculados
     */
    virtual bool excluir(const Identificador& identificador) = 0;

    /**
     * @brief Lista os identificadores de todos os projetos cadastrados
     * @param lista Ponteiro para a lista que sera preenchida
     * @retval true  Listagem realizada
     * @retval false Falha na listagem
     */
    virtual bool listar(list<Identificador>* lista) = 0;
};

/**
 * @brief Interface da apresentacao de projetos
 *
 * Coordena a interacao com o usuario para operacoes sobre projetos.
 */
class IUProjetos {
  public:
    virtual ~IUProjetos() = default;

    /**
     * @brief Executa o menu de operacoes sobre projetos
     * @param email Email do usuario autenticado
     * @param papel Papel do usuario autenticado
     * @retval true  Operacao concluida
     * @retval false Falha
     */
    virtual bool executar(const Email& email, const Papel& papel) = 0;

    /**
     * @brief Define a controladora de servicos de projetos
     * @param cntr Ponteiro para a controladora que implementa IServicosProjetos
     */
    virtual void setCntrServicosProjetos(IServicosProjetos* cntr) = 0;
};

// ---------------------------------------------------------------------------
// Quadros
// ---------------------------------------------------------------------------

/**
 * @brief Interface dos servicos de quadro
 *
 * Oferece as operacoes CRUD e listagem sobre quadros.
 */
class IServicosQuadros {
  public:
    virtual ~IServicosQuadros() = default;

    /**
     * @brief Cria um novo quadro vinculado a um projeto
     * @param quadro  Quadro a ser criado
     * @param projeto Identificador do projeto ao qual o quadro pertence
     * @retval true  Quadro criado
     * @retval false Identificador ja existente ou projeto inexistente
     */
    virtual bool criar(const Quadro& quadro, const Identificador& projeto) = 0;

    /**
     * @brief Le os dados de um quadro
     * @param quadro Ponteiro que entra com o identificador e sai com os dados preenchidos
     * @retval true  Quadro encontrado
     * @retval false Quadro nao encontrado
     */
    virtual bool ler(Quadro* quadro) = 0;

    /**
     * @brief Atualiza os dados de um quadro
     * @param quadro Quadro com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Quadro nao encontrado, novo limite menor que WIP atual ou dados invalidos
     */
    virtual bool atualizar(const Quadro& quadro) = 0;

    /**
     * @brief Exclui um quadro
     * @param identificador Identificador do quadro a excluir
     * @retval true  Excluido com sucesso
     * @retval false Quadro nao encontrado ou possui cartoes vinculados
     */
    virtual bool excluir(const Identificador& identificador) = 0;

    /**
     * @brief Lista os identificadores de todos os quadros cadastrados
     * @param lista Ponteiro para a lista que sera preenchida
     * @retval true  Listagem realizada
     * @retval false Falha na listagem
     */
    virtual bool listar(list<Identificador>* lista) = 0;
};

/**
 * @brief Interface da apresentacao de quadros
 *
 * Coordena a interacao com o usuario para operacoes sobre quadros.
 */
class IUQuadros {
  public:
    virtual ~IUQuadros() = default;

    /**
     * @brief Executa o menu de operacoes sobre quadros
     * @param email Email do usuario autenticado
     * @param papel Papel do usuario autenticado
     * @retval true  Operacao concluida
     * @retval false Falha
     */
    virtual bool executar(const Email& email, const Papel& papel) = 0;

    /**
     * @brief Define a controladora de servicos de quadros
     * @param cntr Ponteiro para a controladora que implementa IServicosQuadros
     */
    virtual void setCntrServicosQuadros(IServicosQuadros* cntr) = 0;
};

// ---------------------------------------------------------------------------
// Cartoes de Atividade
// ---------------------------------------------------------------------------

/**
 * @brief Interface dos servicos de cartao de atividade
 *
 * Oferece CRUD, movimentacao entre colunas e metricas sobre cartoes.
 */
class IServicosCartaoDeAtividade {
  public:
    virtual ~IServicosCartaoDeAtividade() = default;

    /**
     * @brief Cria um novo cartao vinculado a um quadro e a um desenvolvedor
     * @param cartao        Cartao a ser criado (estado deve ser A FAZER)
     * @param quadro        Identificador do quadro ao qual o cartao pertence
     * @param desenvolvedor Email do desenvolvedor responsavel
     * @retval true  Cartao criado
     * @retval false Identificador ja existente, quadro inexistente ou desenvolvedor inexistente
     */
    virtual bool criar(const CartaoDeAtividade& cartao, const Identificador& quadro,
                       const Email& desenvolvedor) = 0;

    /**
     * @brief Le os dados de um cartao
     * @param cartao Ponteiro que entra com o identificador e sai com os dados preenchidos
     * @retval true  Cartao encontrado
     * @retval false Cartao nao encontrado
     */
    virtual bool ler(CartaoDeAtividade* cartao) = 0;

    /**
     * @brief Atualiza nome, descricao, prioridade e tamanho do cartao
     * @param cartao Cartao com identificador existente e novos dados
     * @retval true  Atualizado com sucesso
     * @retval false Cartao nao encontrado ou dados invalidos
     */
    virtual bool atualizar(const CartaoDeAtividade& cartao) = 0;

    /**
     * @brief Exclui um cartao
     * @param identificador Identificador do cartao a excluir
     * @retval true  Excluido com sucesso
     * @retval false Cartao nao encontrado
     */
    virtual bool excluir(const Identificador& identificador) = 0;

    /**
     * @brief Lista os identificadores de todos os cartoes cadastrados
     * @param lista Ponteiro para a lista que sera preenchida
     * @retval true  Listagem realizada
     * @retval false Falha na listagem
     */
    virtual bool listar(list<Identificador>* lista) = 0;

    /**
     * @brief Move um cartao para o proximo estado
     *
     * Transicoes validas: A FAZER -> FAZENDO e FAZENDO -> FEITO.
     * Ao ir para FAZENDO grava inicio=momento; ao ir para FEITO grava termino=momento.
     *
     * @param cartao  Identificador do cartao a mover
     * @param destino Estado de destino
     * @param momento Timestamp do momento da movimentacao
     * @retval true  Cartao movido
     * @retval false Cartao nao encontrado, transicao invalida, WIP excedido ou momento invalido
     */
    virtual bool mover(const Identificador& cartao, const Estado& destino,
                       const Timestamp& momento) = 0;

    /**
     * @brief Le o tempo de ciclo de um cartao (termino - inicio, em minutos)
     * @param[in]  cartao   Identificador do cartao
     * @param[out] minutos  Tempo de ciclo em minutos
     * @retval true  Leitura realizada (cartao em estado FEITO)
     * @retval false Cartao nao encontrado ou nao esta em FEITO
     */
    virtual bool lerTempoCiclo(const Identificador& cartao, double& minutos) = 0;

    /**
     * @brief Le o tempo de ciclo medio dos cartoes FEITO de um quadro
     * @param[in]  quadro  Identificador do quadro
     * @param[out] minutos Tempo de ciclo medio em minutos
     * @retval true  Leitura realizada (ao menos um cartao FEITO no quadro)
     * @retval false Quadro nao encontrado ou sem cartoes FEITO
     */
    virtual bool lerTempoCicloMedio(const Identificador& quadro, double& minutos) = 0;

    /**
     * @brief Le o lead time de um cartao (termino - entrada, em minutos)
     * @param[in]  cartao  Identificador do cartao
     * @param[out] minutos Lead time em minutos
     * @retval true  Leitura realizada (cartao em estado FEITO)
     * @retval false Cartao nao encontrado ou nao esta em FEITO
     */
    virtual bool lerLeadTime(const Identificador& cartao, double& minutos) = 0;

    /**
     * @brief Le o lead time medio dos cartoes FEITO de um quadro
     * @param[in]  quadro  Identificador do quadro
     * @param[out] minutos Lead time medio em minutos
     * @retval true  Leitura realizada (ao menos um cartao FEITO no quadro)
     * @retval false Quadro nao encontrado ou sem cartoes FEITO
     */
    virtual bool lerLeadTimeMedio(const Identificador& quadro, double& minutos) = 0;

    /**
     * @brief Le o WIP (work in progress) de um quadro
     * @param[in]  quadro Identificador do quadro
     * @param[out] wip    Numero de cartoes em estado FAZENDO
     * @retval true  Leitura realizada
     * @retval false Quadro nao encontrado
     */
    virtual bool lerWip(const Identificador& quadro, int& wip) = 0;
};

/**
 * @brief Interface da apresentacao de cartoes de atividade
 *
 * Coordena a interacao com o usuario para operacoes sobre cartoes.
 */
class IUCartaoDeAtividade {
  public:
    virtual ~IUCartaoDeAtividade() = default;

    /**
     * @brief Executa o menu de operacoes sobre cartoes
     * @param email Email do usuario autenticado
     * @param papel Papel do usuario autenticado
     * @retval true  Operacao concluida
     * @retval false Falha
     */
    virtual bool executar(const Email& email, const Papel& papel) = 0;

    /**
     * @brief Define a controladora de servicos de cartoes
     * @param cntr Ponteiro para a controladora que implementa IServicosCartaoDeAtividade
     */
    virtual void setCntrServicosCartaoDeAtividade(IServicosCartaoDeAtividade* cntr) = 0;
};

#endif // KANBAN_INTERFACES_H
