# Sistema Kanban TP1

Gerenciador de tarefas no estilo Kanban em **C++17**.  
Trabalho Pratico de Tecnicas de Programacao 1 — UnB.

---

## O que e este projeto?

E um sistema de linha de comando (terminal) para gerenciar projetos no estilo Kanban.  
Voce cria uma conta, faz login e pode criar **projetos**, **quadros** e **cartoes de atividade**, alem de movimentar os cartoes entre os estados **A FAZER**, **FAZENDO** e **FEITO**.

Existem dois tipos de usuario:
- **Gestor** — pode criar, editar e excluir tudo.
- **Desenvolvedor** — pode visualizar tudo e mover cartoes.

---

## Pre-requisitos

Antes de compilar, voce precisa ter instalado:

### Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install g++ build-essential
```

### Windows
1. Baixe e instale o **Code::Blocks com MinGW** em: https://www.codeblocks.org/downloads/binaries/  
   Escolha o arquivo com `mingw` no nome (ex.: `codeblocks-20.03mingw-setup.exe`).
2. Isso instala automaticamente o compilador `g++`.

### macOS
```bash
xcode-select --install
```

---

## Como compilar

### Opcao 1 — Code::Blocks (recomendado para Windows)

1. Abra o **Code::Blocks**
2. Va em **File > Open** e abra o arquivo `kanban.cbp` desta pasta
3. Pressione **F9** (ou clique em **Build > Build**)
4. O executavel sera gerado em `bin/Debug/kanban` (Linux/macOS) ou `bin\Debug\kanban.exe` (Windows)

### Opcao 2 — Terminal (Linux / macOS)

Abra o terminal dentro da pasta do projeto e execute:

```bash
g++ -std=c++17 -Wall -Wextra -o kanban \
    dominios.cpp entidades.cpp utilitarios.cpp \
    servicos_usuarios.cpp servicos_projetos.cpp \
    servicos_quadros.cpp servicos_cartoes.cpp \
    apresentacao_usuarios.cpp apresentacao_projetos.cpp \
    apresentacao_quadros.cpp apresentacao_cartoes.cpp \
    teste_fumaca.cpp main.cpp
```

Isso cria o executavel `kanban` na pasta atual.

---

## Como executar

### Linux / macOS
```bash
./kanban
```

### Windows (terminal cmd ou PowerShell)
```
kanban.exe
```

### Windows (pelo Code::Blocks)
Pressione **F10** (Run) apos compilar, ou va em **Build > Run**.

---

## Como usar o sistema

Ao iniciar, voce vera o menu:

```
===========================
   Sistema Kanban TP1
===========================

1. Criar conta
2. Entrar (autenticar)
0. Sair
Opcao:
```

### Passo a passo para comecar

**1. Criar uma conta**
- Digite `1` e pressione Enter
- Informe seu email (ex.: `joao@email.com`)
- Informe seu nome completo (ex.: `Joao Silva`)
- Informe uma senha (5 caracteres: letras e numeros, ex.: `abc12`)
- Escolha o papel: `GESTOR` ou `DESENVOLVEDOR`

**2. Fazer login**
- Digite `2` e pressione Enter
- Informe o email e a senha cadastrados

**3. Menu principal (apos login)**
```
=== MENU PRINCIPAL ===
1. Pessoas
2. Projetos
3. Quadros
4. Cartoes
0. Sair (deslogar)
```

**4. Criar um projeto** (apenas Gestor)
- Va em `2. Projetos > Criar projeto`
- Informe identificador (ex.: `PRJ001`), nome, descricao, data de inicio e termino

**5. Criar um quadro** (apenas Gestor)
- Va em `3. Quadros > Criar quadro`
- Informe o identificador do projeto ao qual o quadro pertence
- Informe identificador, nome e limite de WIP (quantos cartoes podem estar em FAZENDO ao mesmo tempo)

**6. Criar um cartao** (apenas Gestor)
- Va em `4. Cartoes > Criar cartao`
- Informe o identificador do quadro, o email do desenvolvedor responsavel, e os dados do cartao
- O cartao e criado automaticamente no estado **A FAZER**

**7. Mover um cartao**
- Va em `4. Cartoes > Mover cartao`
- Informe o identificador do cartao e o estado destino (`FAZENDO` ou `FEITO`)
- Transicoes validas: `A FAZER` → `FAZENDO` → `FEITO`

### Formatos aceitos pelos campos

| Campo | Formato | Exemplo |
|---|---|---|
| Identificador | 5 a 10 letras maiusculas ou digitos | `PRJ01`, `QDR001` |
| Email | padrao email | `nome@dominio.com` |
| Senha | exatamente 5 caracteres (letras + digitos) | `abc12` |
| Papel | exatamente `GESTOR` ou `DESENVOLVEDOR` | `GESTOR` |
| Estado | `A FAZER`, `FAZENDO` ou `FEITO` | `FAZENDO` |
| Prioridade | `ALTA`, `MEDIA` ou `BAIXA` | `ALTA` |
| Tamanho | `GRANDE`, `MEDIO` ou `PEQUENO` | `MEDIO` |
| Timestamp | `D-MES-AAAA-HH:MM` | `5-JAN-2024-09:00` |
| Limite (WIP) | inteiro de 1 a 10 | `3` |

> Se voce digitar um valor invalido, o sistema pede novamente ate voce acertar o formato.

---

## Executar os testes automatizados

O sistema possui **26 testes automatizados** que verificam todas as funcionalidades sem precisar digitar nada.

Para rodar, basta digitar **`T`** no menu inicial (opcao oculta):

```
Opcao: T
```

Saida esperada:

```
========== TESTE DE FUMACA ==========
  [OK]    1 - Criar gestor
  [OK]    2 - Criar desenvolvedor
  [OK]    3 - Criar pessoa duplicada deve falhar
  ...
  [OK]    26 - Excluir cartao, quadro, projeto, dev e gestor em cascata

=====================================
Casos:  26  Passou: 26  Falhou: 0
RESULTADO: PASSED
=====================================
```

Se aparecer `RESULTADO: FAILED`, alguma regra de negocio esta com problema. O numero do caso indica onde.

---

## Estrutura de arquivos do projeto

```
kanban/
├── dominios.h / .cpp               Tipos de dado com validacao (Email, Senha, Nome, ...)
├── entidades.h / .cpp              Pessoa, Projeto, Quadro, CartaoDeAtividade
├── utilitarios.h / .cpp            Funcoes de timestamp
├── interfaces.h                    Contratos entre as camadas (10 interfaces)
│
├── servicos_usuarios.h / .cpp      Logica de negocio de usuarios e autenticacao
├── servicos_projetos.h / .cpp      Logica de negocio de projetos
├── servicos_quadros.h / .cpp       Logica de negocio de quadros
├── servicos_cartoes.h / .cpp       Logica de negocio de cartoes (mover, metricas)
│
├── apresentacao_usuarios.h / .cpp  Menus e entrada de dados de usuarios
├── apresentacao_projetos.h / .cpp  Menus e entrada de dados de projetos
├── apresentacao_quadros.h / .cpp   Menus e entrada de dados de quadros
├── apresentacao_cartoes.h / .cpp   Menus e entrada de dados de cartoes
│
├── teste_fumaca.h / .cpp           26 testes automatizados
├── main.cpp                        Ponto de entrada do programa
│
├── kanban.cbp                      Projeto Code::Blocks
├── Doxyfile                        Configuracao da documentacao automatica
├── doxyfile_changes.txt            Instrucoes para configurar o Doxygen
└── checklist_compilacao.txt        Lista de verificacao antes de entregar
```

---

## Regras do sistema

| Regra | Detalhe |
|---|---|
| Email nao pode ser alterado | E a chave de identificacao da conta |
| Excluir pessoa falha | Se ela for gestor de algum projeto ou dev de algum cartao |
| Excluir projeto falha | Se ele tiver quadros |
| Excluir quadro falha | Se ele tiver cartoes |
| Criar cartao exige estado A FAZER | Nao e possivel criar ja em FAZENDO ou FEITO |
| Mover para FAZENDO falha | Se o numero de cartoes em FAZENDO >= limite do quadro (WIP) |
| Atualizar cartao | Altera apenas nome, descricao, prioridade e tamanho — estado e datas sao preservados |

---

## Metricas disponiveis (menu Cartoes)

| Metrica | O que mede |
|---|---|
| **Tempo de ciclo** | Quanto tempo o cartao ficou em FAZENDO (termino − inicio, em minutos) |
| **Lead time** | Quanto tempo total o cartao levou desde a criacao (termino − entrada, em minutos) |
| **Tempo de ciclo medio** | Media do tempo de ciclo de todos os cartoes FEITO de um quadro |
| **Lead time medio** | Media do lead time de todos os cartoes FEITO de um quadro |
| **WIP** | Numero de cartoes atualmente em FAZENDO em um quadro |

> Tempo de ciclo e lead time so estao disponiveis para cartoes no estado **FEITO**.

---

## Gerar documentacao (opcional)

```bash
doxygen Doxyfile
# abra docs/html/index.html no navegador
```

Consulte [`doxyfile_changes.txt`](doxyfile_changes.txt) para as configuracoes recomendadas.

---

## Arquitetura resumida (para desenvolvedores)

```
Apresentacao  <--[IServicos*]-->  Servico
     |                               |
  CntrIU*                      CntrServicos*
  (menus,                       (regras de
   validacao                     negocio)
   de formato)                       |
                                 Container*
                                 (std::map,
                                  em memoria)
```

As duas camadas so se conhecem pelas interfaces em `interfaces.h`.  
Nenhum codigo de apresentacao acessa diretamente os containers ou aplica regras de negocio.

---

## Observacoes tecnicas

1. `interfaces.h` foi reescrito para corresponder ao contrato do enunciado (assinaturas originais estavam incompletas).
2. A classe `Pessoa` foi adicionada a `entidades.h/.cpp` (nao existia na versao original).
3. `ContainerCartoes::atualizarCompleto()` foi acrescentado para que `mover()` possa gravar estado e timestamps sem violar a semantica de `atualizar()`.
4. `timestampAgora()` retorna o dia sem zero a esquerda (ex.: `5-JAN-2026-09:00`), compativel com o dominio `Timestamp`.
5. Dominios `Nome`, `Email` e `Identificador` possuem permissividades conhecidas que nao foram corrigidas — `dominios.*` nao deve ser alterado salvo instrucao explicita.
