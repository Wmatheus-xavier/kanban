/**
 * @file utilitarios.h
 * @brief Funcoes utilitarias do sistema Kanban
 */

#ifndef KANBAN_UTILITARIOS_H
#define KANBAN_UTILITARIOS_H

#include <string>

using namespace std;

bool ehBissexto(int ano);
bool ehLetra(char c);
bool ehDigito(char c);
int  paraInt(const string& s);
bool ehPontuacao(char c);

/**
 * @brief Converte um timestamp valido para minutos desde 01-JAN-2000-00:00
 * @param timestamp Timestamp no formato DIA-MES-ANO-HH:MM
 * @return Numero de minutos desde 01-JAN-2000-00:00
 */
long timestampParaMinutos(const string& timestamp);

/**
 * @brief Retorna o timestamp atual no formato DIA-MES-ANO-HH:MM
 * @return String com o timestamp do relogio do sistema
 */
string timestampAgora();

#endif // KANBAN_UTILITARIOS_H
