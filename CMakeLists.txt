cmake_minimum_required(VERSION 3.5)
project(mips-simulator)

# Define o padrão C (C23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

# Diretórios de inclusão próprios
include_directories(include)

# Encontra o pacote GTK+ 3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Adiciona os diretórios de inclusão do GTK
include_directories(${GTK3_INCLUDE_DIRS})

# Arquivos fonte do projeto
set(SOURCES
    src/display.c
    src/instruction_encoder.c
    src/main.c
    src/mips_state.c
    src/parser.c
    src/simulator.c
    src/utils.c
    src/ui.c
)

# Cria o executável
add_executable(mips-simulator ${SOURCES})

# Linka as bibliotecas do GTK com o target
target_link_libraries(mips-simulator ${GTK3_LIBRARIES})

# Define opções de compilação (sem as vírgulas)
target_compile_options(mips-simulator PRIVATE -O3 -Wall)
