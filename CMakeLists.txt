cmake_minimum_required(VERSION 3.10)
project(kanban)

set(CMAKE_CXX_STANDARD 17)

add_executable(kanban
        main.cpp
        dominios.cpp
        dominios.h
        entidades.cpp
        entidades.h
        utilitarios.cpp
        utilitarios.h
        interfaces.h
        servicos_usuarios.cpp
        servicos_usuarios.h
        servicos_projetos.cpp
        servicos_projetos.h
        servicos_quadros.cpp
        servicos_quadros.h
        servicos_cartoes.cpp
        servicos_cartoes.h
        apresentacao_usuarios.cpp
        apresentacao_usuarios.h
        apresentacao_projetos.cpp
        apresentacao_projetos.h
        apresentacao_quadros.cpp
        apresentacao_quadros.h
        apresentacao_cartoes.cpp
        apresentacao_cartoes.h
        teste_fumaca.cpp
        teste_fumaca.h
)
