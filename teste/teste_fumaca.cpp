/**
 * @file teste_fumaca.cpp
 * @brief Implementacao dos 26 testes de fumaca do sistema Kanban
 *
 * Roteiro de cada caso numerado:
 *  1  Criar gestor
 *  2  Criar desenvolvedor
 *  3  Criar pessoa duplicada falha
 *  4  Autenticar gestor com credenciais corretas
 *  5  Autenticar com senha errada falha
 *  6  Ler dados de pessoa cadastrada
 *  7  Listar pessoas (espera 2)
 *  8  Atualizar nome de pessoa
 *  9  Criar projeto (vinculado ao gestor)
 * 10  Criar projeto duplicado falha
 * 11  Ler dados de projeto cadastrado
 * 12  Listar projetos (espera 1)
 * 13  Atualizar dados de projeto
 * 14  Criar quadro (vinculado ao projeto)
 * 15  Criar quadro duplicado falha
 * 16  Ler dados de quadro cadastrado
 * 17  Listar quadros (espera 1)
 * 18  Atualizar quadro
 * 19  Criar cartao (vinculado ao quadro e ao dev)
 * 20  Criar cartao duplicado falha
 * 21  Ler dados de cartao cadastrado
 * 22  Listar cartoes (espera 1)
 * 23  Atualizar cartao (nome, descricao, prioridade, tamanho)
 * 24  Mover cartao A FAZER -> FAZENDO; verificar WIP e inicio gravado
 * 25  Mover cartao FAZENDO -> FEITO; verificar WIP, termino, ciclo e lead time
 * 26  Excluir em cascata: cartao, quadro, projeto, desenvolvedor, gestor
 */

#include <iostream>
#include "teste/teste_fumaca.h"
#include "servicos/servicos_usuarios.h"
#include "servicos/servicos_projetos.h"
#include "servicos/servicos_quadros.h"
#include "servicos/servicos_cartoes.h"
#include "utilitarios/utilitarios.h"

using namespace std;

// ---------------------------------------------------------------------------
// Helpers internos de verificacao
// ---------------------------------------------------------------------------

static int sTotal  = 0;
static int sFalhas = 0;

/// Avalia condicao e imprime resultado numerado.
static void verificar(int num, bool condicao, const string& descricao) {
    sTotal++;
    if (condicao) {
        cout << "  [OK]    " << num << " - " << descricao << "\n";
    } else {
        cout << "  [FALHA] " << num << " - " << descricao << "\n";
        sFalhas++;
    }
}

// ---------------------------------------------------------------------------
// executarTesteFumaca
// ---------------------------------------------------------------------------

void executarTesteFumaca() {
    // Zera contadores (permite chamada multipla sem acumular)
    sTotal  = 0;
    sFalhas = 0;

    cout << "\n========== TESTE DE FUMACA ==========\n";

    // -----------------------------------------------------------------------
    // Instanciar containers e controladoras na pilha
    // -----------------------------------------------------------------------
    ContainerPessoas              cntrPessoas;
    ContainerProjetos             cntrProjetos;
    ContainerQuadros              cntrQuadros;
    ContainerCartoes              cntrCartoes;

    CntrServicosAutenticacao      svcAutenticacao(&cntrPessoas);
    CntrServicosPessoas           svcPessoas(&cntrPessoas, &cntrProjetos, &cntrCartoes);
    CntrServicosProjetos          svcProjetos(&cntrProjetos, &cntrPessoas, &cntrQuadros);
    CntrServicosQuadros           svcQuadros(&cntrQuadros, &cntrProjetos, &cntrCartoes);
    CntrServicosCartaoDeAtividade svcCartoes(&cntrCartoes, &cntrQuadros, &cntrPessoas);

    // -----------------------------------------------------------------------
    // Dominio — objetos reutilizados ao longo dos testes
    // -----------------------------------------------------------------------
    Email eGestor; eGestor.setEmail("gestor@kanban.com");
    Senha sGestor; sGestor.setSenha("abc12");
    Email eDev;    eDev.setEmail("dev@kanban.com");
    Senha sDev;    sDev.setSenha("xyz99");

    Identificador idProj; idProj.setValor("PRJ001");
    Identificador idQ;    idQ.setValor("QDR001");
    Identificador idC;    idC.setValor("CAR001");

    // -----------------------------------------------------------------------
    // CASO 1 — Criar gestor
    // -----------------------------------------------------------------------
    {
        Pessoa p;
        Nome  n; n.setNome("Gestor Kanban");
        Papel pp; pp.setPapel("GESTOR");
        p.setEmail(eGestor); p.setNome(n); p.setSenha(sGestor); p.setPapel(pp);
        verificar(1, svcPessoas.criar(p), "Criar gestor");
    }

    // -----------------------------------------------------------------------
    // CASO 2 — Criar desenvolvedor
    // -----------------------------------------------------------------------
    {
        Pessoa p;
        Nome  n; n.setNome("Dev Kanban");
        Papel pp; pp.setPapel("DESENVOLVEDOR");
        p.setEmail(eDev); p.setNome(n); p.setSenha(sDev); p.setPapel(pp);
        verificar(2, svcPessoas.criar(p), "Criar desenvolvedor");
    }

    // -----------------------------------------------------------------------
    // CASO 3 — Criar pessoa com email duplicado deve falhar
    // -----------------------------------------------------------------------
    {
        Pessoa p;
        Nome  n; n.setNome("Duplicado");
        Senha s; s.setSenha("aaa11");
        Papel pp; pp.setPapel("GESTOR");
        p.setEmail(eGestor); p.setNome(n); p.setSenha(s); p.setPapel(pp);
        verificar(3, !svcPessoas.criar(p), "Criar pessoa duplicada deve falhar");
    }

    // -----------------------------------------------------------------------
    // CASO 4 — Autenticar gestor com credenciais corretas
    // -----------------------------------------------------------------------
    {
        Papel papelAuth;
        bool ok = svcAutenticacao.autenticar(eGestor, sGestor, &papelAuth);
        verificar(4, ok && papelAuth.getPapel() == "GESTOR",
                  "Autenticar gestor com senha correta");
    }

    // -----------------------------------------------------------------------
    // CASO 5 — Autenticar com senha errada deve falhar
    // -----------------------------------------------------------------------
    {
        Senha senhaErrada; senhaErrada.setSenha("zzz99");
        Papel papelAuth;
        verificar(5, !svcAutenticacao.autenticar(eGestor, senhaErrada, &papelAuth),
                  "Autenticar com senha errada deve falhar");
    }

    // -----------------------------------------------------------------------
    // CASO 6 — Ler dados de pessoa cadastrada
    // -----------------------------------------------------------------------
    {
        Pessoa p; p.setEmail(eGestor);
        bool ok = svcPessoas.ler(&p);
        verificar(6, ok && p.getNome().getNome() == "Gestor Kanban",
                  "Ler dados do gestor");
    }

    // -----------------------------------------------------------------------
    // CASO 7 — Listar pessoas (espera 2)
    // -----------------------------------------------------------------------
    {
        list<Email> lista;
        svcPessoas.listar(&lista);
        verificar(7, lista.size() == 2, "Listar 2 pessoas cadastradas");
    }

    // -----------------------------------------------------------------------
    // CASO 8 — Atualizar nome de pessoa
    // -----------------------------------------------------------------------
    {
        Pessoa p; p.setEmail(eGestor);
        svcPessoas.ler(&p);
        Nome novoNome; novoNome.setNome("Gestor Novo");
        p.setNome(novoNome);
        bool upd = svcPessoas.atualizar(p);
        Pessoa pCheck; pCheck.setEmail(eGestor);
        svcPessoas.ler(&pCheck);
        verificar(8, upd && pCheck.getNome().getNome() == "Gestor Novo",
                  "Atualizar nome de pessoa");
    }

    // -----------------------------------------------------------------------
    // CASO 9 — Criar projeto vinculado ao gestor
    // -----------------------------------------------------------------------
    {
        Projeto proj;
        Nome    n;  n.setNome("Projeto Alpha");
        Texto   d;  d.setValor("Descricao do projeto.");
        Timestamp ini; ini.setValor("1-JAN-2024-08:00");
        Timestamp fim; fim.setValor("31-DEZ-2024-23:59");
        proj.setIdentificador(idProj); proj.setNome(n);
        proj.setDescricao(d); proj.setInicio(ini); proj.setTermino(fim);
        verificar(9, svcProjetos.criar(proj, eGestor), "Criar projeto");
    }

    // -----------------------------------------------------------------------
    // CASO 10 — Criar projeto duplicado deve falhar
    // -----------------------------------------------------------------------
    {
        Projeto proj;
        Nome    n; n.setNome("Duplicado");
        Texto   d; d.setValor("Duplicado.");
        Timestamp ini; ini.setValor("1-JAN-2024-08:00");
        Timestamp fim; fim.setValor("31-DEZ-2024-23:59");
        proj.setIdentificador(idProj); proj.setNome(n);
        proj.setDescricao(d); proj.setInicio(ini); proj.setTermino(fim);
        verificar(10, !svcProjetos.criar(proj, eGestor),
                  "Criar projeto duplicado deve falhar");
    }

    // -----------------------------------------------------------------------
    // CASO 11 — Ler dados de projeto cadastrado
    // -----------------------------------------------------------------------
    {
        Projeto p; p.setIdentificador(idProj);
        bool ok = svcProjetos.ler(&p);
        verificar(11, ok && p.getNome().getNome() == "Projeto Alpha",
                  "Ler dados do projeto");
    }

    // -----------------------------------------------------------------------
    // CASO 12 — Listar projetos (espera 1)
    // -----------------------------------------------------------------------
    {
        list<Identificador> lista;
        svcProjetos.listar(&lista);
        verificar(12, lista.size() == 1, "Listar 1 projeto cadastrado");
    }

    // -----------------------------------------------------------------------
    // CASO 13 — Atualizar dados de projeto
    // -----------------------------------------------------------------------
    {
        Projeto p; p.setIdentificador(idProj);
        svcProjetos.ler(&p);
        Nome novoNome; novoNome.setNome("Projeto Beta");
        p.setNome(novoNome);
        bool upd = svcProjetos.atualizar(p);
        Projeto pCheck; pCheck.setIdentificador(idProj);
        svcProjetos.ler(&pCheck);
        verificar(13, upd && pCheck.getNome().getNome() == "Projeto Beta",
                  "Atualizar nome de projeto");
    }

    // -----------------------------------------------------------------------
    // CASO 14 — Criar quadro vinculado ao projeto
    // -----------------------------------------------------------------------
    {
        Quadro q;
        Nome   n; n.setNome("Quadro Sprint");
        Limite l; l.setLimite(3);
        q.setIdentificador(idQ); q.setNome(n); q.setLimite(l);
        verificar(14, svcQuadros.criar(q, idProj), "Criar quadro");
    }

    // -----------------------------------------------------------------------
    // CASO 15 — Criar quadro duplicado deve falhar
    // -----------------------------------------------------------------------
    {
        Quadro q;
        Nome   n; n.setNome("Duplicado");
        Limite l; l.setLimite(2);
        q.setIdentificador(idQ); q.setNome(n); q.setLimite(l);
        verificar(15, !svcQuadros.criar(q, idProj),
                  "Criar quadro duplicado deve falhar");
    }

    // -----------------------------------------------------------------------
    // CASO 16 — Ler dados de quadro cadastrado
    // -----------------------------------------------------------------------
    {
        Quadro q; q.setIdentificador(idQ);
        bool ok = svcQuadros.ler(&q);
        verificar(16, ok && q.getLimite().getLimite() == 3,
                  "Ler dados do quadro (limite 3)");
    }

    // -----------------------------------------------------------------------
    // CASO 17 — Listar quadros (espera 1)
    // -----------------------------------------------------------------------
    {
        list<Identificador> lista;
        svcQuadros.listar(&lista);
        verificar(17, lista.size() == 1, "Listar 1 quadro cadastrado");
    }

    // -----------------------------------------------------------------------
    // CASO 18 — Atualizar quadro (limite >= WIP atual=0)
    // -----------------------------------------------------------------------
    {
        Quadro q; q.setIdentificador(idQ);
        svcQuadros.ler(&q);
        Limite novoLimite; novoLimite.setLimite(5);
        q.setLimite(novoLimite);
        bool upd = svcQuadros.atualizar(q);
        Quadro qCheck; qCheck.setIdentificador(idQ);
        svcQuadros.ler(&qCheck);
        verificar(18, upd && qCheck.getLimite().getLimite() == 5,
                  "Atualizar limite do quadro para 5");
    }

    // -----------------------------------------------------------------------
    // CASO 19 — Criar cartao vinculado ao quadro e ao dev
    // -----------------------------------------------------------------------
    {
        CartaoDeAtividade c;
        Nome      n;   n.setNome("Tarefa Um");
        Texto     d;   d.setValor("Implementar funcionalidade.");
        Prioridade pr; pr.setPrioridade("ALTA");
        Estado    est; est.setEstado("A FAZER");
        Tamanho   tam; tam.setTamanho("MEDIO");
        Timestamp ent; ent.setValor("2-JAN-2024-09:00");
        c.setIdentificador(idC); c.setNome(n); c.setDescricao(d);
        c.setPrioridade(pr); c.setEstado(est); c.setTamanho(tam);
        c.setEntrada(ent); c.setInicio(ent); c.setTermino(ent);
        verificar(19, svcCartoes.criar(c, idQ, eDev), "Criar cartao");
    }

    // -----------------------------------------------------------------------
    // CASO 20 — Criar cartao duplicado deve falhar
    // -----------------------------------------------------------------------
    {
        CartaoDeAtividade c;
        Nome      n;   n.setNome("Duplicado");
        Texto     d;   d.setValor("Dup.");
        Prioridade pr; pr.setPrioridade("BAIXA");
        Estado    est; est.setEstado("A FAZER");
        Tamanho   tam; tam.setTamanho("PEQUENO");
        Timestamp ent; ent.setValor("2-JAN-2024-09:00");
        c.setIdentificador(idC); c.setNome(n); c.setDescricao(d);
        c.setPrioridade(pr); c.setEstado(est); c.setTamanho(tam);
        c.setEntrada(ent); c.setInicio(ent); c.setTermino(ent);
        verificar(20, !svcCartoes.criar(c, idQ, eDev),
                  "Criar cartao duplicado deve falhar");
    }

    // -----------------------------------------------------------------------
    // CASO 21 — Ler dados de cartao cadastrado
    // -----------------------------------------------------------------------
    {
        CartaoDeAtividade c; c.setIdentificador(idC);
        bool ok = svcCartoes.ler(&c);
        verificar(21, ok && c.getEstado().getEstado() == "A FAZER",
                  "Ler cartao; estado inicial A FAZER");
    }

    // -----------------------------------------------------------------------
    // CASO 22 — Listar cartoes (espera 1)
    // -----------------------------------------------------------------------
    {
        list<Identificador> lista;
        svcCartoes.listar(&lista);
        verificar(22, lista.size() == 1, "Listar 1 cartao cadastrado");
    }

    // -----------------------------------------------------------------------
    // CASO 23 — Atualizar cartao (nome, descricao, prioridade, tamanho)
    // -----------------------------------------------------------------------
    {
        CartaoDeAtividade c; c.setIdentificador(idC);
        svcCartoes.ler(&c);
        Nome      novoNome; novoNome.setNome("Tarefa Atualiz");
        Texto     novaDesc; novaDesc.setValor("Descricao atualizada.");
        Prioridade novaPri; novaPri.setPrioridade("BAIXA");
        Tamanho   novoTam; novoTam.setTamanho("PEQUENO");
        c.setNome(novoNome); c.setDescricao(novaDesc);
        c.setPrioridade(novaPri); c.setTamanho(novoTam);
        bool upd = svcCartoes.atualizar(c);
        CartaoDeAtividade cCheck; cCheck.setIdentificador(idC);
        svcCartoes.ler(&cCheck);
        verificar(23,
            upd &&
            cCheck.getNome().getNome() == "Tarefa Atualiz" &&
            cCheck.getEstado().getEstado() == "A FAZER",
            "Atualizar cartao preserva estado A FAZER");
    }

    // -----------------------------------------------------------------------
    // CASO 24 — Mover A FAZER -> FAZENDO; verificar WIP e inicio gravado
    // -----------------------------------------------------------------------
    {
        Estado  fazendo; fazendo.setEstado("FAZENDO");
        Timestamp tFaz;  tFaz.setValor("3-JAN-2024-10:00");
        bool ok = svcCartoes.mover(idC, fazendo, tFaz);

        int wip = -1;
        svcCartoes.lerWip(idQ, wip);

        CartaoDeAtividade cCheck; cCheck.setIdentificador(idC);
        svcCartoes.ler(&cCheck);

        verificar(24,
            ok &&
            wip == 1 &&
            cCheck.getEstado().getEstado() == "FAZENDO" &&
            cCheck.getInicio().getValor() == "3-JAN-2024-10:00",
            "Mover A FAZER->FAZENDO; WIP=1; inicio gravado");
    }

    // -----------------------------------------------------------------------
    // CASO 25 — Mover FAZENDO -> FEITO; verificar WIP, termino, ciclo, lead
    // -----------------------------------------------------------------------
    {
        Estado  feito; feito.setEstado("FEITO");
        Timestamp tFei; tFei.setValor("5-JAN-2024-18:00");
        bool ok = svcCartoes.mover(idC, feito, tFei);

        int wip = -1;
        svcCartoes.lerWip(idQ, wip);

        // termino - inicio = 5-JAN-2024-18:00 - 3-JAN-2024-10:00
        //   = 2 dias e 8 h = 2*1440 + 480 = 3360 min
        double ciclo = -1.0;
        svcCartoes.lerTempoCiclo(idC, ciclo);

        // termino - entrada = 5-JAN-2024-18:00 - 2-JAN-2024-09:00
        //   = 3 dias e 9 h = 3*1440 + 540 = 4860 min
        double lead = -1.0;
        svcCartoes.lerLeadTime(idC, lead);

        double mediaCiclo = -1.0, mediaLead = -1.0;
        svcCartoes.lerTempoCicloMedio(idQ, mediaCiclo);
        svcCartoes.lerLeadTimeMedio(idQ, mediaLead);

        verificar(25,
            ok &&
            wip == 0 &&
            ciclo == 3360.0 &&
            lead  == 4860.0 &&
            mediaCiclo == 3360.0 &&
            mediaLead  == 4860.0,
            "Mover FAZENDO->FEITO; WIP=0; ciclo=3360; lead=4860; medias corretas");
    }

    // -----------------------------------------------------------------------
    // CASO 26 — Excluir em cascata: cartao, quadro, projeto, dev, gestor
    // -----------------------------------------------------------------------
    {
        bool exclCartao  = svcCartoes.excluir(idC);
        bool exclQuadro  = svcQuadros.excluir(idQ);
        bool exclProjeto = svcProjetos.excluir(idProj);
        bool exclDev     = svcPessoas.excluir(eDev);
        bool exclGestor  = svcPessoas.excluir(eGestor);

        verificar(26,
            exclCartao && exclQuadro && exclProjeto && exclDev && exclGestor,
            "Excluir cartao, quadro, projeto, dev e gestor em cascata");
    }

    // -----------------------------------------------------------------------
    // Resultado final
    // -----------------------------------------------------------------------
    cout << "\n=====================================\n";
    cout << "Casos:  " << sTotal
         << "  Passou: " << (sTotal - sFalhas)
         << "  Falhou: " << sFalhas << "\n";
    cout << "RESULTADO: " << (sFalhas == 0 ? "PASSED" : "FAILED") << "\n";
    cout << "=====================================\n";
}
