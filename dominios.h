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
 * Formato válido:  6 caracteres, sendo os três primeiros letras (a-z ou A-Z) e os três últimos dígitos (0-9)
*/
class Identificador {
    private:
        string valor;
        void validar(const string&);
    public:
        /**
         * @brief Defini o valor do identificador
         * @param texto no formato valido
         * @throw invalid argument se o formato for inválido
         */
        void setValor(const string&);

        /**
         * @brief Obtém o valor do identificador
         * @return texto com o valor armazenado
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
         * @brief Defini o valor do nome
         * @param texto no formato valido
         * @throw invalid argument se o formato for inválido
        */
        void setNome(const string& nome);

        /**
         * @brief Obtém o valor do nome
         * @return texto com o valor armazenado
        */
        string getNome() const;
};

/**
 * @brief Dominio Texto
 *
 * Formato válido: texto de até 30 caracteres. Pode conter letras, dígitos, espaços e sinais de pontuação (vírgula, ponto e vírgula, ponto, dois pontos, interrogação e exclamação). O primeiro caractere é letra maiúscula, o último é ponto e um sinal de pontuação não pode ser seguido por outro
*/
class Texto {
    private:
        string valor;
        void validar(const string&);
    public:
        /**
         * @brief Defini o valor do nome
         * @param texto no formato valido
         * @throw invalid argument se o formato for inválido
        */
        void setValor(const string&);

        /**
         * @brief Obtém o valor do texto
         * @return texto com o valor armazenado
        */
        string getValor() const;
};


/**
 * @brief Timestamp
 *
 *Formato válido:  * Formato DIA-MES-ANO-HH:MM, onde DIA é de 1 a 31, MES é a sigla (JAN, FEV, MAR, ABR, MAI, JUN, JUL, AGO, SET, OUT, NOV, DEZ), ANO é de 2000 a 2099 e o horário é de 00:00 a 23:59. A data deve ser válida, considerando anos bissextos
*/
class Timestamp {
    private:
        string valor;
        void validar(const string&);
    public:
        /**
         * @brief Defini o valor do timestamp
         * @param texto no formato valido
         * @throw invalid argument se o formato for inválido
        */
        void setValor(const string&);

        /**
         * @brief Obtém o valor do timestamp
         * @return texto com o valor armazenado
        */
        string getValor() const;
};

// ------------------------------------------------------------
// Dominio: Limite
// Regra: Valor inteiro no intervalo de 1 a 25
// ------------------------------------------------------------
/**
 * @brief Limite
 *
 * Formato válido:  Valor inteiro de 1 a 25, que designa o máximo de atividades simultâneas na coluna FAZENDO
 */
class Limite {
    private:
        static constexpr int LIMITE_MIN = 1;
        static constexpr int LIMITE_MAX = 25;
        int valor = 1;
        static void validar(int valor);
    public:
        /**
         * @brief Defini o valor do limite
         * @param inteiro no formato valido
         * @throw invalid argument se o formato for inválido
        */
        void setLimite(int valor);

        /**
         * @brief Obtém o valor do timestamp
         * @return inteiro com o valor armazenado
        */
        int getLimite() const;
};

/**
 * @brief Dominio Prioridade
 *
 * Formato válido: "ALTA", "MEDIA" ou "BAIXA"
 */

class Prioridade {
    private:
        inline static constexpr array<const char *, 3> OPCOES = { "ALTA", "MEDIA", "BAIXA" };
        string prioridade;
        static void validar(const string& prioridade);
    public:
        /**
         * @brief Define a prioridade
         * @param texto entre as opções "ALTA", "MEDIA" ou "BAIXA"
         * @throws invalid argument se o valor não for uma das opções
         */
        void setPrioridade(const string& prioridade);

        /**
         * @brief Obtém o valor da prioridade
         * @return texto com o valor armazenado
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
         * @param texto com um dos valores "A FAZER", "FAZENDO" ou "FEITO"
         * @throws invalid argument se o valor não for uma das opções
         */
        void setEstado(const string& estado);

        /**
          * @brief Obtém o valor do estado
          * @return texto com o valor armazenado
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
         * @param texto com um dos valores "GRANDE", "MEDIO" ou "PEQUENO"
         * @throws invalid argument se o valor não for uma das opções
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
 * Formato válido: parte-local@domínio. A parte local pode conter letras minúsculas (a-z), dígitos (0-9), ponto e hífen; não pode iniciar nem terminar com ponto ou hífen, ponto ou hífen deve ser seguido por letra ou dígito e o comprimento máximo é 64 caracteres. O domínio é composto por uma ou mais partes separadas por ponto; cada parte pode conter letras minúsculas (a-z), dígitos (0-9) ou hífen, não pode iniciar nem terminar com hífen e o comprimento máximo do domínio é 255 caracteres
 */
class Email {
    private:
        string email;
        static void validar(const string& email);
    public:
        /**
         * @brief Define o valor do email
         * @param email texto no formato válido
         * @throws invalid_argument se o formato for inválido
         */
        void setEmail(const string& email);

        /**
         * @brief Obtém o valor do email
         * @return texto com o valor armazenado
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
         * @param senha texto no formato válido
         * @throws invalid_argument se o formato for inválido
         */
        void setSenha(const string& senha);

        /**
         * @brief Obtém o valor da senha
         * @return texto com o valor armazenado
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
         * @param papel texto com um dos valores "GESTOR" ou "DESENVOLVEDOR"
         * @throws invalid_argument se o valor não for uma das opções
         */
        void setPapel(const string& papel);

        /**
         * @brief Obtém o papel
         * @return texto com o papel armazenado
         */
        string getPapel() const;
};

#endif // KANBAN_DOMINIOS_H
