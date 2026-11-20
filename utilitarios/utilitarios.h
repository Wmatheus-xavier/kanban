/**
 * @file utilitarios.h
 * @brief Funcoes utilitarias do sistema Kanban
 */

#ifndef KANBAN_UTILITARIOS_H
#define KANBAN_UTILITARIOS_H

#include <string>

using namespace std;

/**
 * @brief Verifica se um ano é bissexto
 * @param ano Ano a verificar
 * @return true se o ano for bissexto
 */
bool ehBissexto(int ano);

/**
 * @brief Verifica se um caractere é letra (a-z ou A-Z)
 * @param c Caractere a verificar
 * @return true se for letra
 */
bool ehLetra(char c);

/**
 * @brief Verifica se um caractere é dígito (0-9)
 * @param c Caractere a verificar
 * @return true se for dígito
 */
bool ehDigito(char c);

/**
 * @brief Converte um texto numérico em inteiro
 * @param s Texto contendo apenas dígitos
 * @return Valor inteiro correspondente
 */
int paraInt(const string& s);

/**
 * @brief Verifica se um caractere é sinal de pontuação
 * @param c Caractere a verificar
 * @return true se for pontuação
 */
bool ehPontuacao(char c);

/**
 * @brief Converte um timestamp válido para minutos desde 01-JAN-2000-00:00
 * @param ts Timestamp no formato DIA-MES-ANO-HH:MM
 * @return Número de minutos desde 01-JAN-2000-00:00
 */
long timestampParaMinutos(const string& ts);

/**
 * @brief Retorna o timestamp atual no formato DIA-MES-ANO-HH:MM
 * @return Texto com o timestamp do relógio do sistema
 */
string timestampAgora();

#endif // KANBAN_UTILITARIOS_H
