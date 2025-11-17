# Guia Rápido - Usando uv 🚀

Este projeto usa **uv**, um gerenciador de pacotes Python extremamente rápido (10-100x mais rápido que pip!).

## 🎯 Setup Rápido (Recomendado)

### Linux/macOS

```bash
./setup.sh
```

### Windows

```cmd
setup.bat
```

## 📦 Instalação Manual

### 1. Instalar uv

**Linux/macOS:**

```bash
curl -LsSf https://astral.sh/uv/install.sh | sh
```

**Windows (PowerShell):**

```powershell
irm https://astral.sh/uv/install.ps1 | iex
```

**Ou via pipx:**

```bash
pipx install uv
```

### 2. Criar Ambiente Virtual

```bash
uv venv
```

### 3. Ativar Ambiente Virtual

**Linux/macOS:**

```bash
source .venv/bin/activate
```

**Windows (CMD):**

```cmd
.venv\Scripts\activate
```

**Windows (PowerShell):**

```powershell
.venv\Scripts\Activate.ps1
```

### 4. Instalar Dependências

**Apenas produção:**

```bash
uv pip install -e .
```

**Com dependências de desenvolvimento:**

```bash
uv pip install -e ".[dev]"
```

## 🏃 Executar o Projeto

```bash
# Ativar ambiente (se não estiver ativo)
source .venv/bin/activate  # Linux/macOS
# ou
.venv\Scripts\activate     # Windows

# Executar o exemplo
python LT-3F-765kv.py
```

## 🛠️ Comandos de Desenvolvimento

```bash
# Formatar código
make format
# ou
black campelmag/ LT-3F-765kv.py

# Verificar código (linting)
make lint
# ou
ruff check campelmag/ LT-3F-765kv.py

# Corrigir problemas automaticamente
make lint-fix
# ou
ruff check --fix campelmag/ LT-3F-765kv.py

# Verificar tipos
make type-check
# ou
mypy campelmag/

# Ver todos os comandos disponíveis
make help
```

## ⚡ Por que uv?

- **10-100x mais rápido** que pip
- **Gerenciamento de ambiente integrado** (não precisa de venv separado)
- **Cache global** - instala pacotes uma vez, usa em todos os projetos
- **Compatível com pip** - usa os mesmos índices (PyPI)
- **Resolução de dependências superior**
- **Escrito em Rust** - extremamente eficiente

## 📊 Comparação de Velocidade

```
Instalando numpy, scipy, matplotlib:

pip:          ~45 segundos
uv:           ~3 segundos
```

## 🔄 Sincronizar Dependências

Se você clonou o projeto:

```bash
# Criar venv e instalar tudo
uv venv
source .venv/bin/activate
uv pip install -e ".[dev]"
```

## 🧹 Limpar Cache

```bash
# Limpar arquivos Python temporários
make clean

# Limpar cache do uv
uv cache clean
```

## 📝 Adicionar Nova Dependência

1. Adicione em `pyproject.toml`:

```toml
[project]
dependencies = [
    "numpy>=1.26.0",
    "scipy>=1.11.0",
    "matplotlib>=3.8.0",
    "nova-lib>=1.0.0",  # Nova dependência
]
```

2. Reinstale:

```bash
uv pip install -e .
```

## 🔍 Inspecionar Pacotes Instalados

```bash
# Listar pacotes
uv pip list

# Mostrar informações de um pacote
uv pip show numpy

# Verificar dependências
uv pip tree
```

## 🆘 Problemas Comuns

### uv não encontrado após instalação

Adicione ao PATH ou reinicie o terminal.

### Permissão negada no Linux/macOS

```bash
chmod +x setup.sh
./setup.sh
```

### Python não encontrado

uv detecta automaticamente Python. Se necessário:

```bash
uv venv --python 3.12
```

## 📚 Mais Informações

- Documentação oficial: https://github.com/astral-sh/uv
- Este projeto: Ver README.md para mais detalhes
