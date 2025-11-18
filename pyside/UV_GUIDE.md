# Guia do UV - Gerenciador de Pacotes Python

## O que é uv?

[uv](https://github.com/astral-sh/uv) é um gerenciador de pacotes e ambientes Python extremamente rápido, escrito em Rust pela Astral (criadores do Ruff). É um substituto moderno para pip, pip-tools, pipx, poetry, pyenv, e virtualenv.

### Por que usar uv?

- ⚡ **10-100x mais rápido** que pip
- 🔒 **Lockfile automático** para builds reproduzíveis
- 🎯 **Interface simples** e intuitiva
- 🔄 **Compatível** com pip e requirements.txt
- 📦 **Gerencia Python** e dependências
- 🚀 **Sem configuração** complexa

## Instalação do uv

### Windows (PowerShell)

```powershell
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"
```

### Linux/macOS

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
```

### Via pipx

```bash
pipx install uv
```

### Verificar instalação

```bash
uv --version
```

## Uso Básico no CampelMag

### 1. Configuração Inicial (Setup)

```bash
# Use o script fornecido
setup.bat      # Windows
bash setup.sh  # Linux/macOS

# Ou manualmente
uv sync
```

O comando `uv sync` irá:

- Criar ambiente virtual em `.venv/`
- Ler `pyproject.toml`
- Instalar todas as dependências
- Criar `uv.lock` para builds reproduzíveis

### 2. Executar a Aplicação

```bash
# Executar diretamente (SEM ativar ambiente)
uv run python -m campelmag_gui.main

# Ou usar o comando registrado
uv run campelmag-gui

# Executar qualquer script Python
uv run python exemplo.py
```

### 3. Ativar Ambiente Virtual (Opcional)

```bash
# Linux/macOS
source .venv/bin/activate

# Windows
.venv\Scripts\activate

# Depois de ativado, use normalmente
python -m campelmag_gui.main
campelmag-gui
```

### 4. Adicionar Novas Dependências

```bash
# Adicionar pacote
uv add nome-do-pacote

# Adicionar pacote de desenvolvimento
uv add --dev black

# Adicionar versão específica
uv add "numpy>=2.0.0"
```

### 5. Remover Dependências

```bash
uv remove nome-do-pacote
```

### 6. Atualizar Dependências

```bash
# Atualizar todas
uv lock --upgrade

# Atualizar pacote específico
uv lock --upgrade-package numpy
```

## Comandos Úteis

### Gerenciamento de Ambiente

```bash
# Criar novo ambiente
uv venv

# Criar com Python específico
uv venv --python 3.12

# Listar pacotes instalados
uv pip list

# Verificar dependências
uv pip check
```

### Executar Scripts

```bash
# Executar script Python
uv run python script.py

# Executar com argumentos
uv run python script.py --arg valor

# Executar comando instalado
uv run black .
```

### Sincronização

```bash
# Sincronizar com pyproject.toml
uv sync

# Sincronizar apenas dependências principais (sem dev)
uv sync --no-dev

# Sincronizar tudo, incluindo dev
uv sync --all-extras
```

## Comparação: pip vs uv

### Com pip (tradicional)

```bash
python -m venv venv
source venv/bin/activate  # ou venv\Scripts\activate
pip install --upgrade pip
pip install -r requirements.txt
python -m campelmag_gui.main
```

### Com uv (moderno)

```bash
uv sync
uv run campelmag-gui
```

## Estrutura de Arquivos

```
pyside/
├── .venv/              # Ambiente virtual (criado por uv)
├── pyproject.toml      # Dependências e configuração
├── uv.lock            # Lockfile (versionamento exato)
├── requirements.txt    # Compatibilidade com pip
└── ...
```

## Dicas e Truques

### 1. Executar sem instalar globalmente

```bash
# Executar ferramenta temporária
uvx black .
uvx ruff check .
```

### 2. Usar Python específico

```bash
# Criar ambiente com Python 3.12
uv venv --python 3.12

# Ou especificar no pyproject.toml
# requires-python = ">=3.12"
```

### 3. Cache compartilhado

uv mantém um cache global de pacotes, tornando instalações subsequentes instantâneas.

### 4. Modo offline

```bash
# Usar cache offline
uv sync --offline
```

### 5. Exportar para requirements.txt

```bash
uv pip compile pyproject.toml -o requirements.txt
```

## Migração de venv para uv

Se você tem um projeto existente com venv:

```bash
# 1. Remover venv antigo (opcional)
rm -rf venv  # Linux/macOS
# ou
rmdir /s venv  # Windows

# 2. Sincronizar com uv
uv sync

# 3. Pronto! Use .venv/ em vez de venv/
```

## Solução de Problemas

### uv não encontrado após instalação

**Windows**: Reinicie o PowerShell ou adicione ao PATH manualmente.

**Linux/macOS**: Execute `source $HOME/.cargo/env` ou reinicie o terminal.

### Conflito de versões

```bash
# Limpar cache
uv cache clean

# Recriar ambiente
rm -rf .venv
uv sync
```

### Dependências não sincronizam

```bash
# Forçar reinstalação
uv sync --reinstall
```

## Recursos Adicionais

- 📚 [Documentação Oficial](https://docs.astral.sh/uv/)
- 🔗 [GitHub Repository](https://github.com/astral-sh/uv)
- 💬 [Discord da Astral](https://astral.sh/discord)

## Resumo de Comandos

| Ação                | Comando uv                | Equivalente pip                   |
| ------------------- | ------------------------- | --------------------------------- |
| Instalar deps       | `uv sync`                 | `pip install -r requirements.txt` |
| Executar script     | `uv run python script.py` | `python script.py`                |
| Adicionar pacote    | `uv add pacote`           | `pip install pacote`              |
| Remover pacote      | `uv remove pacote`        | `pip uninstall pacote`            |
| Listar pacotes      | `uv pip list`             | `pip list`                        |
| Criar ambiente      | `uv venv`                 | `python -m venv venv`             |
| Executar ferramenta | `uvx black .`             | `pipx run black .`                |

---

**Nota**: O arquivo `requirements.txt` é mantido para compatibilidade, mas `pyproject.toml` é a fonte de verdade quando usando uv.
