#!/usr/bin/env bash
# Script de setup rápido para o projeto usando uv

set -e

echo "🚀 Configurando projeto campelmag..."

# Verifica se uv está instalado
if ! command -v uv &> /dev/null; then
    echo "📦 Instalando uv..."
    curl -LsSf https://astral.sh/uv/install.sh | sh
    echo "✅ uv instalado com sucesso!"
fi

# Cria ambiente virtual
echo "🔧 Criando ambiente virtual..."
uv venv

# Ativa o ambiente virtual
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    source .venv/Scripts/activate
else
    source .venv/bin/activate
fi

# Instala dependências
echo "📚 Instalando dependências..."
uv pip install -e ".[dev]"

echo ""
echo "✅ Setup completo!"
echo ""
echo "Para ativar o ambiente virtual:"
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    echo "  .venv\\Scripts\\activate"
else
    echo "  source .venv/bin/activate"
fi
echo ""
echo "Para executar o exemplo:"
echo "  python LT-3F-765kv.py"
echo ""
echo "Comandos úteis:"
echo "  make help          - Ver todos os comandos disponíveis"
echo "  make run           - Executar o exemplo"
echo "  make format        - Formatar código"
echo "  make lint          - Verificar código"
echo "  make type-check    - Verificar tipos"
