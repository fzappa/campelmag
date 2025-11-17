@echo off
REM Script de setup rápido para o projeto usando uv no Windows

echo 🚀 Configurando projeto campelmag...

REM Verifica se uv está instalado
where uv >nul 2>nul
if %errorlevel% neq 0 (
    echo 📦 Instalando uv...
    powershell -c "irm https://astral.sh/uv/install.ps1 | iex"
    echo ✅ uv instalado com sucesso!
)

REM Cria ambiente virtual
echo 🔧 Criando ambiente virtual...
uv venv

REM Ativa o ambiente virtual
call .venv\Scripts\activate.bat

REM Instala dependências
echo 📚 Instalando dependências...
uv pip install -e .[dev]

echo.
echo ✅ Setup completo!
echo.
echo Para ativar o ambiente virtual:
echo   .venv\Scripts\activate
echo.
echo Para executar o exemplo:
echo   python LT-3F-765kv.py
echo.
echo Comandos úteis:
echo   make help          - Ver todos os comandos disponíveis
echo   make run           - Executar o exemplo
echo   make format        - Formatar código
echo   make lint          - Verificar código
echo   make type-check    - Verificar tipos
