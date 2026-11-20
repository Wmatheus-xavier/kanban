cmake_minimum_required(VERSION 3.10)
project(kanban)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kanban
        main.cpp

        dominio/dominios.cpp
        dominio/dominios.h

        entidades/entidades.cpp
        entidades/entidades.h

        interfaces/interfaces.h

        utilitarios/utilitarios.cpp
        utilitarios/utilitarios.h

        servicos/servicos_usuarios.cpp
        servicos/servicos_usuarios.h
        servicos/servicos_projetos.cpp
        servicos/servicos_projetos.h
        servicos/servicos_quadros.cpp
        servicos/servicos_quadros.h
        servicos/servicos_cartoes.cpp
        servicos/servicos_cartoes.h

        apresentacao/apresentacao_usuarios.cpp
        apresentacao/apresentacao_usuarios.h
        apresentacao/apresentacao_projetos.cpp
        apresentacao/apresentacao_projetos.h
        apresentacao/apresentacao_quadros.cpp
        apresentacao/apresentacao_quadros.h
        apresentacao/apresentacao_cartoes.cpp
        apresentacao/apresentacao_cartoes.h

        teste/teste_fumaca.cpp
        teste/teste_fumaca.h
)


target_include_directories(kanban PRIVATE
        ${CMAKE_SOURCE_DIR}
)
