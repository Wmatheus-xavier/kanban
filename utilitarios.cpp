#include "utilitarios.h"
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

bool ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

bool ehLetra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool ehDigito(char c) {
    return c >= '0' && c <= '9';
}

int paraInt(const string& s) {
    if (s.empty()) {
        throw invalid_argument("Numero invalido.");
    }
    int r = 0;
    for (size_t i = 0; i < s.size(); i++) {
        if (!ehDigito(s[i])) {
            throw invalid_argument("Numero invalido.");
        }
        r = r * 10 + (s[i] - '0');
    }
    return r;
}

bool ehPontuacao(char c) {
    return c == ',' || c == ';' || c == '.' ||
           c == ':' || c == '?' || c == '!';
}

// Meses em ordem (indice 0 = JAN, 11 = DEZ)
static const string MESES[12] = {
    "JAN","FEV","MAR","ABR","MAI","JUN",
    "JUL","AGO","SET","OUT","NOV","DEZ"
};

// Dias em cada mes (nao-bissexto)
static const int DIAS_MES[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static int indiceMes(const string& mes) {
    for (int i = 0; i < 12; i++) {
        if (MESES[i] == mes) return i;
    }
    return -1;
}

/**
 * @brief Converte um timestamp valido para minutos desde 01-JAN-2000-00:00
 *
 * Formato: DIA-MES-ANO-HH:MM  (ex: 1-JAN-2000-00:00 = 0)
 */
long timestampParaMinutos(const string& ts) {
    // Parse: partes separadas por '-'
    // Formato: DIA-MES-ANO-HH:MM
    // Encontrar posicoes
    size_t p1 = ts.find('-');
    size_t p2 = ts.find('-', p1 + 1);
    size_t p3 = ts.find('-', p2 + 1);

    int dia  = paraInt(ts.substr(0, p1));
    string mes = ts.substr(p1 + 1, p2 - p1 - 1);
    int ano  = paraInt(ts.substr(p2 + 1, p3 - p2 - 1));
    string hm = ts.substr(p3 + 1); // HH:MM
    size_t pc = hm.find(':');
    int hora = paraInt(hm.substr(0, pc));
    int min  = paraInt(hm.substr(pc + 1));

    int idxMes = indiceMes(mes);

    // Contar dias desde 01-JAN-2000
    long totalDias = 0;

    // Anos completos de 2000 ate (ano-1)
    for (int a = 2000; a < ano; a++) {
        totalDias += ehBissexto(a) ? 366 : 365;
    }

    // Meses completos do ano corrente
    for (int m = 0; m < idxMes; m++) {
        totalDias += DIAS_MES[m];
        if (m == 1 && ehBissexto(ano)) totalDias++;
    }

    // Dias do mes corrente (1-based: dia 1 = 0 dias completos)
    totalDias += (dia - 1);

    return totalDias * 24L * 60L + hora * 60L + min;
}

/**
 * @brief Retorna o timestamp atual no formato DIA-MES-ANO-HH:MM
 */
string timestampAgora() {
    time_t agora = time(nullptr);
    struct tm* t = localtime(&agora);

    int dia  = t->tm_mday;
    int mes  = t->tm_mon;      // 0-based
    int ano  = t->tm_year + 1900;
    int hora = t->tm_hour;
    int min  = t->tm_min;

    ostringstream oss;
    oss << dia << "-" << MESES[mes] << "-" << ano << "-"
        << setw(2) << setfill('0') << hora << ":"
        << setw(2) << setfill('0') << min;
    return oss.str();
}
