#!/bin/bash
# Script de configuração para Linux/macOS usando uv

echo "========================================"
echo "CampelMag GUI - Setup (uv)"
echo "========================================"

# Verificar uv
if ! command -v uv &> /dev/null; then
    echo "ERRO: uv não encontrado!"
    echo ""
    echo "Instalando uv..."
    curl -LsSf https://astral.sh/uv/install.sh | sh
    echo ""
    echo "uv instalado! Por favor, reinicie o terminal e execute setup.sh novamente."
    exit 1
fi

echo ""
echo "Sincronizando dependências com uv..."
uv sync

echo ""
echo "========================================"
echo "Instalação concluída!"
echo "========================================"
echo ""
echo "Para executar a aplicação:"
echo "  uv run python -m campelmag_gui.main"
echo ""
echo "Ou use o comando direto:"
echo "  uv run campelmag-gui"
echo ""
echo "Para ativar o ambiente virtual:"
echo "  source .venv/bin/activate"
echo ""
