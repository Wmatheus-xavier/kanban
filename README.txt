====================================================================
  SISTEMA KANBAN TP1 -- DOCUMENTACAO DE ARQUITETURA
  Tecnicas de Programacao 1 -- UnB
====================================================================

--------------------------------------------------------------------
VISAO GERAL
--------------------------------------------------------------------
Sistema de gerenciamento de tarefas no estilo Kanban implementado
em C++17, compativel com Code::Blocks.  A arquitetura separa
responsabilidades em duas camadas comunicantes apenas por interfaces
(classes abstratas com metodos virtuais puros).

Camada de Apresentacao  -- interage com o usuario via cin / cout;
                           valida formato dos dados de entrada
                           usando os dominios (try/catch em loop).
Camada de Servico       -- aplica regras de negocio; armazena
                           entidades em memoria (std::map).

--------------------------------------------------------------------
DIAGRAMA DE CAMADAS (PlantUML)
--------------------------------------------------------------------

@startuml camadas
skinparam packageStyle rectangle
skinparam componentStyle uml2

package "Apresentacao" {
    [CntrIUAutenticacao]
    [CntrIUPessoas]
    [CntrIUProjetos]
    [CntrIUQuadros]
    [CntrIUCartaoDeAtividade]
}

package "Interfaces" {
    interface IServicosAutenticacao
    interface IServicosPessoas
    interface IServicosProjetos
    interface IServicosQuadros
    interface IServicosCartaoDeAtividade
    interface IUAutenticacao
    interface IUPessoas
    interface IUProjetos
    interface IUQuadros
    interface IUCartaoDeAtividade
}

package "Servico" {
    [CntrServicosAutenticacao]
    [CntrServicosPessoas]
    [CntrServicosProjetos]
    [CntrServicosQuadros]
    [CntrServicosCartaoDeAtividade]
    [ContainerPessoas]
    [ContainerProjetos]
    [ContainerQuadros]
    [ContainerCartoes]
}

package "Dominio" {
    [Identificador]
    [Nome]
    [Texto]
    [Timestamp]
    [Limite]
    [Prioridade]
    [Estado]
    [Tamanho]
    [Email]
    [Senha]
    [Papel]
}

package "Entidades" {
    [Pessoa]
    [Projeto]
    [Quadro]
    [CartaoDeAtividade]
}

[CntrIUAutenticacao]       ..> IUAutenticacao
[CntrIUPessoas]            ..> IUPessoas
[CntrIUProjetos]           ..> IUProjetos
[CntrIUQuadros]            ..> IUQuadros
[CntrIUCartaoDeAtividade]  ..> IUCartaoDeAtividade

[CntrIUAutenticacao]       --> IServicosAutenticacao
[CntrIUPessoas]            --> IServicosPessoas
[CntrIUProjetos]           --> IServicosProjetos
[CntrIUQuadros]            --> IServicosQuadros
[CntrIUCartaoDeAtividade]  --> IServicosCartaoDeAtividade

[CntrServicosAutenticacao]      ..> IServicosAutenticacao
[CntrServicosPessoas]           ..> IServicosPessoas
[CntrServicosProjetos]          ..> IServicosProjetos
[CntrServicosQuadros]           ..> IServicosQuadros
[CntrServicosCartaoDeAtividade] ..> IServicosCartaoDeAtividade

[CntrServicosAutenticacao]      --> ContainerPessoas
[CntrServicosPessoas]           --> ContainerPessoas
[CntrServicosPessoas]           --> ContainerProjetos
[CntrServicosPessoas]           --> ContainerCartoes
[CntrServicosProjetos]          --> ContainerProjetos
[CntrServicosProjetos]          --> ContainerPessoas
[CntrServicosProjetos]          --> ContainerQuadros
[CntrServicosQuadros]           --> ContainerQuadros
[CntrServicosQuadros]           --> ContainerProjetos
[CntrServicosQuadros]           --> ContainerCartoes
[CntrServicosCartaoDeAtividade] --> ContainerCartoes
[CntrServicosCartaoDeAtividade] --> ContainerQuadros
[CntrServicosCartaoDeAtividade] --> ContainerPessoas
@enduml

--------------------------------------------------------------------
DIAGRAMA DE CLASSES DE DOMINIO (PlantUML)
--------------------------------------------------------------------

@startuml dominio
class Identificador {
    -valor : string
    +setValor(string)
    +getValor() : string
}
class Nome {
    -nome : string
    +setNome(string)
    +getNome() : string
}
class Texto {
    -valor : string
    +setValor(string)
    +getValor() : string
}
class Timestamp {
    -valor : string
    +setValor(string)
    +getValor() : string
}
class Limite {
    -limite : int
    +setLimite(int)
    +getLimite() : int
}
class Prioridade {
    -prioridade : string
    +setPrioridade(string)
    +getPrioridade() : string
}
class Estado {
    -estado : string
    +setEstado(string)
    +getEstado() : string
}
class Tamanho {
    -tamanho : string
    +setTamanho(string)
    +getTamanho() : string
}
class Email {
    -email : string
    +setEmail(string)
    +getEmail() : string
}
class Senha {
    -senha : string
    +setSenha(string)
    +getSenha() : string
}
class Papel {
    -papel : string
    +setPapel(string)
    +getPapel() : string
}

note "Cada set() lanca\ninvalid_argument se\nformato invalido" as N1
Identificador .. N1
@enduml

--------------------------------------------------------------------
DIAGRAMA DE ENTIDADES (PlantUML)
--------------------------------------------------------------------

@startuml entidades
class Pessoa {
    -email : Email
    -nome  : Nome
    -senha : Senha
    -papel : Papel
}
class Projeto {
    -identificador : Identificador
    -nome          : Nome
    -descricao     : Texto
    -inicio        : Timestamp
    -termino       : Timestamp
}
class Quadro {
    -identificador : Identificador
    -nome          : Nome
    -limite        : Limite
}
class CartaoDeAtividade {
    -identificador : Identificador
    -nome          : Nome
    -descricao     : Texto
    -prioridade    : Prioridade
    -estado        : Estado
    -tamanho       : Tamanho
    -entrada       : Timestamp
    -inicio        : Timestamp
    -termino       : Timestamp
}

Pessoa "1 (gestor)"    o-- "0..*" Projeto      : gerencia
Pessoa "1 (dev)"       o-- "0..*" CartaoDeAtividade : responsavel
Projeto                o-- "0..*" Quadro        : contem
Quadro                 o-- "0..*" CartaoDeAtividade : contem

note "Relacoes armazenadas\nnos Containers\n(nao nas entidades)" as N2
Projeto .. N2
@enduml

--------------------------------------------------------------------
FLUXO PRINCIPAL (PlantUML -- sequencia simplificada)
--------------------------------------------------------------------

@startuml fluxo_login
actor Usuario
participant "main()" as Main
participant CntrIUAutenticacao as IUA
participant CntrServicosAutenticacao as SVA
participant ContainerPessoas as CP

Usuario -> Main : opcao "2" (Entrar)
Main -> IUA : autenticar(&email, &papel)
IUA -> Usuario : solicita email e senha
Usuario -> IUA : email, senha
IUA -> SVA : autenticar(email, senha, &papel)
SVA -> CP : pesquisar(&pessoa)
CP --> SVA : true + dados
SVA --> IUA : true + papel
IUA --> Main : true + email + papel
Main -> Main : exibe menu pos-login
@enduml

--------------------------------------------------------------------
FLUXO DE MOVIMENTACAO DE CARTAO (PlantUML -- sequencia)
--------------------------------------------------------------------

@startuml fluxo_mover
actor Usuario
participant CntrIUCartaoDeAtividade as IUC
participant CntrServicosCartaoDeAtividade as SVC
participant ContainerCartoes as CC
participant ContainerQuadros as CQ

Usuario -> IUC : opcao "Mover cartao"
IUC -> SVC : mover(idCartao, destino, timestampAgora())
SVC -> CC : pesquisar(&cartao)
CC --> SVC : cartao atual
SVC -> CQ : pesquisar(&quadro)
CQ --> SVC : quadro + limite
SVC -> CC : listarPorQuadro(idQuadro)
CC --> SVC : lista de cartoes
SVC -> SVC : calcular WIP
alt estado == A_FAZER e destino == FAZENDO
    alt WIP >= limite
        SVC --> IUC : false (WIP excedido)
    else
        SVC -> CC : atualizarCompleto(cartao com inicio=momento)
        SVC --> IUC : true
    end
else estado == FAZENDO e destino == FEITO
    SVC -> CC : atualizarCompleto(cartao com termino=momento)
    SVC --> IUC : true
end
IUC --> Usuario : mensagem de resultado
@enduml

--------------------------------------------------------------------
TABELA DE RASTREABILIDADE FUNCIONALIDADE x ARQUIVO
--------------------------------------------------------------------

Funcionalidade                    | Papel    | Arquivo de apresentacao      | Arquivo de servico
----------------------------------|----------|------------------------------|---------------------------
1  Criar conta (Pessoa)           | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
2  Autenticar                     | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
3  Ler Pessoa                     | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
4  Atualizar Pessoa                | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
5  Excluir Pessoa                  | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
6  Listar Pessoas                  | Ambos    | apresentacao_usuarios.cpp    | servicos_usuarios.cpp
7  Criar Projeto                   | Gestor   | apresentacao_projetos.cpp    | servicos_projetos.cpp
8  Ler Projeto                     | Ambos    | apresentacao_projetos.cpp    | servicos_projetos.cpp
9  Atualizar Projeto               | Gestor   | apresentacao_projetos.cpp    | servicos_projetos.cpp
10 Excluir Projeto                 | Gestor   | apresentacao_projetos.cpp    | servicos_projetos.cpp
11 Listar Projetos                 | Ambos    | apresentacao_projetos.cpp    | servicos_projetos.cpp
12 Criar Quadro                    | Gestor   | apresentacao_quadros.cpp     | servicos_quadros.cpp
13 Ler Quadro                      | Ambos    | apresentacao_quadros.cpp     | servicos_quadros.cpp
14 Atualizar Quadro                | Gestor   | apresentacao_quadros.cpp     | servicos_quadros.cpp
15 Excluir Quadro                  | Gestor   | apresentacao_quadros.cpp     | servicos_quadros.cpp
16 Listar Quadros                  | Ambos    | apresentacao_quadros.cpp     | servicos_quadros.cpp
17 Criar Cartao                    | Gestor   | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
18 Ler Cartao                      | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
19 Atualizar Cartao                | Gestor   | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
20 Excluir Cartao                  | Gestor   | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
21 Listar Cartoes                  | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
22 Mover Cartao                    | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
23 Ler Tempo de Ciclo              | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
24 Ler Tempo de Ciclo Medio        | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
25 Ler Lead Time                   | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
26 Ler Lead Time Medio             | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp
   Ler WIP (funcao interna)        | Ambos    | apresentacao_cartoes.cpp     | servicos_cartoes.cpp

--------------------------------------------------------------------
REGRAS DE NEGOCIO IMPLEMENTADAS
--------------------------------------------------------------------

Autenticacao
  - email + senha comparados diretamente; papel retornado em parametro de saida.

Pessoas
  - Email e chave primaria imutavel.
  - Excluir falha se a pessoa e gestor de algum projeto.
  - Excluir falha se a pessoa e desenvolvedor de algum cartao.

Projetos
  - Gestor informado em criar() deve existir em ContainerPessoas.
  - Excluir falha se o projeto tem ao menos um quadro vinculado.

Quadros
  - Projeto informado em criar() deve existir em ContainerProjetos.
  - Atualizar falha se novo limite < WIP atual do quadro.
  - Excluir falha se o quadro tem ao menos um cartao vinculado.

Cartoes
  - Quadro e desenvolvedor informados em criar() devem existir.
  - Estado inicial obrigatorio: "A FAZER".
  - Atualizar preserva estado, entrada, inicio, termino e relacoes.
  - Mover A FAZER -> FAZENDO: falha se WIP >= limite; grava inicio.
  - Mover FAZENDO -> FEITO: falha se momento < inicio; grava termino.
  - Tempo de ciclo e lead time: apenas para cartoes em FEITO.
  - Medias: apenas se houver ao menos um cartao FEITO no quadro.

--------------------------------------------------------------------
ESTRUTURA DE ARQUIVOS
--------------------------------------------------------------------

kanban/
|-- dominios.h / .cpp          Tipos de dominio com validacao
|-- entidades.h / .cpp         Pessoa, Projeto, Quadro, CartaoDeAtividade
|-- utilitarios.h / .cpp       timestampParaMinutos(), timestampAgora()
|-- interfaces.h               10 interfaces puras (contrato fixo)
|-- servicos_usuarios.h / .cpp ContainerPessoas + Autenticacao + Pessoas
|-- servicos_projetos.h / .cpp ContainerProjetos + CntrServicosProjetos
|-- servicos_quadros.h / .cpp  ContainerQuadros + CntrServicosQuadros
|-- servicos_cartoes.h / .cpp  ContainerCartoes + CntrServicosCartaoDeAtividade
|-- apresentacao_usuarios.h / .cpp CntrIUAutenticacao + CntrIUPessoas
|-- apresentacao_projetos.h / .cpp CntrIUProjetos
|-- apresentacao_quadros.h / .cpp  CntrIUQuadros
|-- apresentacao_cartoes.h / .cpp  CntrIUCartaoDeAtividade
|-- teste_fumaca.h / .cpp      26 casos de teste sem cin/cout interativo
|-- main.cpp                   Ponto de entrada; opcao oculta "T" => testes
|-- kanban.cbp                 Projeto Code::Blocks
|-- Doxyfile                   Configuracao Doxygen
|-- doxyfile_changes.txt       Alteracoes recomendadas ao Doxyfile
|-- checklist_compilacao.txt   Lista de verificacao pre-entrega
|-- README.txt                 Este arquivo

--------------------------------------------------------------------
COMO COMPILAR (linha de comando)
--------------------------------------------------------------------

  g++ -std=c++17 -Wall -Wextra -o kanban \
      dominios.cpp entidades.cpp utilitarios.cpp \
      servicos_usuarios.cpp servicos_projetos.cpp \
      servicos_quadros.cpp servicos_cartoes.cpp \
      apresentacao_usuarios.cpp apresentacao_projetos.cpp \
      apresentacao_quadros.cpp apresentacao_cartoes.cpp \
      teste_fumaca.cpp main.cpp

--------------------------------------------------------------------
COMO EXECUTAR O TESTE DE FUMACA
--------------------------------------------------------------------

  ./kanban
  (menu) T  <Enter>

  Saida esperada:
    Casos:  26  Passou: 26  Falhou: 0
    RESULTADO: PASSED

--------------------------------------------------------------------
OBSERVACOES E DIVERGENCIAS ENCONTRADAS
--------------------------------------------------------------------

1. interfaces.h original tinha assinaturas incompletas (sem Email/
   Papel/list<> nos parametros). O arquivo foi reescrito respeitando
   o contrato especificado no prompt; as assinaturas originais nao
   foram mantidas.

2. entidades.h/entidades.cpp originais nao continham a classe Pessoa.
   A classe foi adicionada seguindo o mesmo padrao das demais entidades.

3. ContainerCartoes::atualizar() preserva estado/timestamps (conforme
   especificacao), mas mover() precisa alterar esses campos. Foi
   adicionado o metodo atualizarCompleto() para uso exclusivo de
   CntrServicosCartaoDeAtividade::mover(). Este metodo extra nao faz
   parte do contrato de interfaces mas e necessario para a implementacao
   correta das regras de negocio.

4. timestampAgora() retorna dia sem zero a esquerda (ex: "5-JAN-2024-
   09:00"). O dominio Timestamp aceita 1 ou 2 digitos para o dia,
   portanto esta compativel.

5. Os dominios Nome, Email e Identificador tem permissividades
   documentadas mas nao corrigidas (nao alterar dominios.* salvo
   instrucao explicita):
   - Nome: "espaco deve ser seguido de letra" nao e verificado alem
     de duplo-espaco.
   - Email: aceita "a.-b@x.com" (ponto seguido de hifen).
   - Email: aceita dominio sem ponto (ex: "a@b").

====================================================================
