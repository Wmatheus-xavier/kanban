/**
 * @file main.cpp
 * @brief Ponto de entrada do sistema Kanban
 *
 * Instancia todos os containers e controladoras na pilha, conecta as camadas
 * via ponteiros para interfaces e roda o fluxo principal.
 *
 * Menu inicial:
 *   1. Criar conta
 *   2. Autenticar (login)
 *   T  Teste de fumaca (opcao oculta)
 *   0. Sair
 *
 * Apos login: menu principal com Pessoas, Projetos, Quadros, Cartoes e Sair.
 */

#include <iostream>
#include "servicos_usuarios.h"
#include "servicos_projetos.h"
#include "servicos_quadros.h"
#include "servicos_cartoes.h"
#include "apresentacao_usuarios.h"
#include "apresentacao_projetos.h"
#include "apresentacao_quadros.h"
#include "apresentacao_cartoes.h"
#include "teste_fumaca.h"
#include "utilitarios.h"

using namespace std;

int main() {
    // -----------------------------------------------------------------------
    // 1. Instanciar containers (objetos na pilha; sem new/delete)
    // -----------------------------------------------------------------------
    ContainerPessoas   cntrPessoas;
    ContainerProjetos  cntrProjetos;
    ContainerQuadros   cntrQuadros;
    ContainerCartoes   cntrCartoes;

    // -----------------------------------------------------------------------
    // 2. Instanciar controladoras de servico (injecao de containers)
    // -----------------------------------------------------------------------
    CntrServicosAutenticacao      svcAutenticacao(&cntrPessoas);
    CntrServicosPessoas           svcPessoas(&cntrPessoas, &cntrProjetos, &cntrCartoes);
    CntrServicosProjetos          svcProjetos(&cntrProjetos, &cntrPessoas, &cntrQuadros);
    CntrServicosQuadros           svcQuadros(&cntrQuadros, &cntrProjetos, &cntrCartoes);
    CntrServicosCartaoDeAtividade svcCartoes(&cntrCartoes, &cntrQuadros, &cntrPessoas);

    // -----------------------------------------------------------------------
    // 3. Instanciar controladoras de apresentacao
    // -----------------------------------------------------------------------
    CntrIUAutenticacao      iuAutenticacao;
    CntrIUPessoas           iuPessoas;
    CntrIUProjetos          iuProjetos;
    CntrIUQuadros           iuQuadros;
    CntrIUCartaoDeAtividade iuCartoes;

    // -----------------------------------------------------------------------
    // 4. Injetar dependencias via interfaces
    // -----------------------------------------------------------------------
    iuAutenticacao.setCntrServicosAutenticacao(&svcAutenticacao);
    iuPessoas.setCntrServicosPessoas(&svcPessoas);
    iuProjetos.setCntrServicosProjetos(&svcProjetos);
    iuQuadros.setCntrServicosQuadros(&svcQuadros);
    iuCartoes.setCntrServicosCartaoDeAtividade(&svcCartoes);

    // -----------------------------------------------------------------------
    // 5. Loop principal
    // -----------------------------------------------------------------------
    cout << "===========================\n";
    cout << "   Sistema Kanban TP1\n";
    cout << "===========================\n";

    string entrada;
    bool sair = false;
    while (!sair) {
        cout << "\n1. Criar conta\n";
        cout << "2. Entrar (autenticar)\n";
        cout << "0. Sair\n";
        cout << "Opcao: ";
        getline(cin, entrada);

        if (entrada == "0") {
            sair = true;
            cout << "Encerrando. Ate logo!\n";

        } else if (entrada == "1") {
            iuPessoas.cadastrar();

        } else if (entrada == "2") {
            Email  emailLogado;
            Papel  papelLogado;
            if (!iuAutenticacao.autenticar(&emailLogado, &papelLogado)) continue;

            // ----- Menu principal pos-login -----
            bool deslogar = false;
            while (!deslogar) {
                cout << "\n=== MENU PRINCIPAL ===\n";
                cout << "1. Pessoas\n";
                cout << "2. Projetos\n";
                cout << "3. Quadros\n";
                cout << "4. Cartoes\n";
                cout << "0. Sair (deslogar)\n";
                cout << "Opcao: ";
                string opc;
                getline(cin, opc);
                if (opc == "0") {
                    deslogar = true;
                } else if (opc == "1") {
                    iuPessoas.executar(emailLogado, papelLogado);
                } else if (opc == "2") {
                    iuProjetos.executar(emailLogado, papelLogado);
                } else if (opc == "3") {
                    iuQuadros.executar(emailLogado, papelLogado);
                } else if (opc == "4") {
                    iuCartoes.executar(emailLogado, papelLogado);
                } else {
                    cout << "Opcao invalida.\n";
                }
            }

        } else if (entrada == "T" || entrada == "t") {
            // Opcao oculta: executa o teste de fumaca
            executarTesteFumaca();

        } else {
            cout << "Opcao invalida.\n";
        }
    }

    return 0;
}
