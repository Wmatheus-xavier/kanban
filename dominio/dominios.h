/**
 * @file dominios.h
 * @brief Declaração das classes de domínio do sistema Kanban
 */

#ifndef KANBAN_DOMINIOS_H
#define KANBAN_DOMINIOS_H

#include <string>
#include <array>
#include <stdexcept>

using namespace std;

/**
 * @brief Domínio Identificador
 *
 * Formato válido: 6 caracteres, sendo os três primeiros letras (a-z ou A-Z) e os três últimos dígitos (0-9)
 */
class Identificador {
private:
    string valor;
    void validar(const string&);
public:
    /**
     * @brief Define o valor do identificador
     * @param v Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setValor(const string& v);

    /**
     * @brief Obtém o valor do identificador
     * @return Texto com o valor armazenado
     */
    string getValor() const;
};

/**
 * @brief Domínio Nome
 *
 * Formato válido: texto de 1 a 15 caracteres, contendo apenas letras (a-z ou A-Z) e espaços em branco. Não pode iniciar nem terminar com espaço, e o espaço deve ser seguido por letra
 */
class Nome {
private:
    static constexpr int NOME_MIN = 1;
    static constexpr int NOME_MAX = 15;
    string nome;
    static void validar(const string& nome);
public:
    /**
     * @brief Define o valor do nome
     * @param nome Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setNome(const string& nome);

    /**
     * @brief Obtém o valor do nome
     * @return Texto com o valor armazenado
     */
    string getNome() const;
};

/**
 * @brief Domínio Texto
 *
 * Formato válido: texto de até 30 caracteres. Pode conter letras, dígitos, espaços e sinais de pontuação (vírgula, ponto e vírgula, ponto, dois pontos, interrogação e exclamação). O primeiro caractere é letra maiúscula, o último é ponto e um sinal de pontuação não pode ser seguido por outro
 */
class Texto {
private:
    string valor;
    void validar(const string&);
public:
    /**
     * @brief Define o valor do texto
     * @param v Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setValor(const string& v);

    /**
     * @brief Obtém o valor do texto
     * @return Texto com o valor armazenado
     */
    string getValor() const;
};

/**
 * @brief Domínio Timestamp
 *
 * Formato válido: DIA-MES-ANO-HH:MM, onde DIA é de 1 a 31, MES é a sigla (JAN, FEV, MAR, ABR, MAI, JUN, JUL, AGO, SET, OUT, NOV, DEZ), ANO é de 2000 a 2099 e o horário é de 00:00 a 23:59. A data deve ser válida, considerando anos bissextos
 */
class Timestamp {
private:
    string valor;
    void validar(const string&);
public:
    /**
     * @brief Define o valor do timestamp
     * @param v Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setValor(const string& v);

    /**
     * @brief Obtém o valor do timestamp
     * @return Texto com o valor armazenado
     */
    string getValor() const;
};

/**
 * @brief Domínio Limite
 *
 * Formato válido: valor inteiro de 1 a 25, que designa o máximo de atividades simultâneas na coluna FAZENDO
 */
class Limite {
private:
    static constexpr int LIMITE_MIN = 1;
    static constexpr int LIMITE_MAX = 25;
    int valor = 1;
    static void validar(int valor);
public:
    /**
     * @brief Define o valor do limite
     * @param valor Inteiro no formato válido
     * @throws invalid_argument se o valor for inválido
     */
    void setLimite(int valor);

    /**
     * @brief Obtém o valor do limite
     * @return Inteiro com o valor armazenado
     */
    int getLimite() const;
};

/**
 * @brief Domínio Prioridade
 *
 * Valores válidos: "ALTA", "MEDIA" ou "BAIXA"
 */
class Prioridade {
private:
    inline static constexpr array<const char *, 3> OPCOES = { "ALTA", "MEDIA", "BAIXA" };
    string prioridade;
    static void validar(const string& prioridade);
public:
    /**
     * @brief Define a prioridade
     * @param prioridade Texto com um dos valores "ALTA", "MEDIA" ou "BAIXA"
     * @throws invalid_argument se o valor não for uma das opções
     */
    void setPrioridade(const string& prioridade);

    /**
     * @brief Obtém o valor da prioridade
     * @return Texto com o valor armazenado
     */
    string getPrioridade() const;
};

/**
 * @brief Domínio Estado
 *
 * Valores válidos: "A FAZER", "FAZENDO" ou "FEITO"
 */
class Estado {
private:
    inline static constexpr array<const char *, 3> OPCOES = { "A FAZER", "FAZENDO", "FEITO" };
    string estado;
    static void validar(const string& estado);
public:
    /**
     * @brief Define o estado
     * @param estado Texto com um dos valores "A FAZER", "FAZENDO" ou "FEITO"
     * @throws invalid_argument se o valor não for uma das opções
     */
    void setEstado(const string& estado);

    /**
     * @brief Obtém o valor do estado
     * @return Texto com o valor armazenado
     */
    string getEstado() const;
};

/**
 * @brief Domínio Tamanho
 *
 * Valores válidos: "GRANDE", "MEDIO" ou "PEQUENO"
 */
class Tamanho {
private:
    inline static constexpr array<const char *, 3> OPCOES = { "GRANDE", "MEDIO", "PEQUENO" };
    string tamanho;
    static void validar(const string& tamanho);
public:
    /**
     * @brief Define o tamanho
     * @param tamanho Texto com um dos valores "GRANDE", "MEDIO" ou "PEQUENO"
     * @throws invalid_argument se o valor não for uma das opções
     */
    void setTamanho(const string& tamanho);

    /**
     * @brief Obtém o tamanho
     * @return Texto com o tamanho armazenado
     */
    string getTamanho() const;
};

/**
 * @brief Domínio Email
 *
 * Formato válido: parte-local\@domínio. A parte local pode conter letras minúsculas (a-z), dígitos (0-9), ponto e hífen; não pode iniciar nem terminar com ponto ou hífen, ponto ou hífen deve ser seguido por letra ou dígito e o comprimento máximo é 64 caracteres. O domínio é composto por uma ou mais partes separadas por ponto; cada parte pode conter letras minúsculas (a-z), dígitos (0-9) ou hífen, não pode iniciar nem terminar com hífen e o comprimento máximo do domínio é 255 caracteres
 */
class Email {
private:
    string email;
    static void validar(const string& email);
public:
    /**
     * @brief Define o valor do email
     * @param email Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setEmail(const string& email);

    /**
     * @brief Obtém o valor do email
     * @return Texto com o valor armazenado
     */
    string getEmail() const;
};

/**
 * @brief Domínio Senha
 *
 * Formato válido: 5 caracteres, sendo cada um letra (a-z ou A-Z) ou dígito (0-9), com pelo menos uma letra e pelo menos um dígito
 */
class Senha {
private:
    string senha;
    static void validar(const string& senha);
public:
    /**
     * @brief Define o valor da senha
     * @param senha Texto no formato válido
     * @throws invalid_argument se o formato for inválido
     */
    void setSenha(const string& senha);

    /**
     * @brief Obtém o valor da senha
     * @return Texto com o valor armazenado
     */
    string getSenha() const;
};

/**
 * @brief Domínio Papel
 *
 * Valores válidos: "GESTOR" ou "DESENVOLVEDOR"
 */
class Papel {
private:
    inline static constexpr array<const char *, 2> OPCOES = { "GESTOR", "DESENVOLVEDOR" };
    string papel;
    static void validar(const string& papel);
public:
    /**
     * @brief Define o papel
     * @param papel Texto com um dos valores "GESTOR" ou "DESENVOLVEDOR"
     * @throws invalid_argument se o valor não for uma das opções
     */
    void setPapel(const string& papel);

    /**
     * @brief Obtém o papel
     * @return Texto com o papel armazenado
     */
    string getPapel() const;
};

#endif // KANBAN_DOMINIOS_H
