# mips-simulator

# Instruções de build do Projeto

Este guia explica como compilar e executar o projeto usando CMake e Make.

## Pré-requisitos

Certifique-se de que as seguintes ferramentas estão instaladas no seu sistema:

1. **CMake**: Para gerar os arquivos de build.
2. **Make**: Para compilar e vincular o projeto.
3. **C Compiler**: GCC ou outro compilador equivalente.

Para verificar se essas ferramentas estão instaladas, execute:
```bash
cmake --version
gcc --version
make --version
```
Se algum desses comandos falhar, instale as ferramentas ausentes.

### Instalando Dependências
- **Ubuntu/Debian**:
  ```bash
  sudo apt update
  sudo apt install -y build-essential cmake
  ```
- **Fedora**:
  ```bash
  sudo dnf install -y gcc gcc-c++ make cmake
  ```
- **Arch Linux**:
  ```bash
  sudo pacman -Syu base-devel cmake
  ```

## Compilar e Executar Instruções

1. **Clonar o repositório** (se você ainda não fez isso):
   ```bash
   git clone <repository-url>
   cd <repository-folder>
   ```

2. **Criar um diretório de build**:
   ```bash
   mkdir build && cd build
   ```

3. **Gerar o sistema de build usando CMake**:
   ```bash
   cmake ..
   ```
   This will generate the necessary Makefiles in the `build` directory.

4. **Compilar o projeto usando Make**:
   ```bash
   make
   ```

5. **Executar o programa**:
   ```bash
   ./mips_simulator
   ```

## Limpar build

Para limpar todos os arquivos gerados e reconstruir o projeto do zero:

1. **Remover o build diretório**:
   ```bash
   rm -rf build
   ```

2. **Repetir o processo de build** começando do passo 2.

---