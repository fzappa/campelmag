# 🚀 Início Rápido - Campelmag com uv

## Setup em 3 Passos

### Linux/macOS

```bash
./setup.sh
source .venv/bin/activate
python LT-3F-765kv.py
```

### Windows

```cmd
setup.bat
.venv\Scripts\activate
python LT-3F-765kv.py
```

## Ou Manualmente

```bash
# 1. Instalar uv
curl -LsSf https://astral.sh/uv/install.sh | sh  # Linux/macOS
# ou
irm https://astral.sh/uv/install.ps1 | iex      # Windows PowerShell

# 2. Criar ambiente e instalar
uv venv
source .venv/bin/activate                        # Linux/macOS
# ou
.venv\Scripts\activate                          # Windows

# 3. Instalar dependências
uv pip install -e ".[dev]"

# 4. Executar
python LT-3F-765kv.py
```

## Comandos Úteis

```bash
make help          # Ver todos os comandos
make run           # Executar exemplo
make format        # Formatar código
make lint          # Verificar código
make type-check    # Verificar tipos
make clean         # Limpar cache
```

## Por que uv?

- ⚡ **10-100x mais rápido** que pip
- 🎯 **Simples** - compatível com pip/requirements.txt
- 🔒 **Confiável** - resolução de dependências superior
- 💾 **Eficiente** - cache global de pacotes
- 🦀 **Rápido** - escrito em Rust

## Mais Informações

- 📖 **Guia Completo:** `UV_GUIDE.md`
- 📝 **Migração:** `MIGRATION_SUMMARY.md`
- 📚 **Documentação:** `README.md`

---

**Desenvolvido com ❤️ usando ferramentas modernas de Python**
