@echo off
REM Script de configuração para Windows usando uv

echo ========================================
echo CampelMag GUI - Setup (uv)
echo ========================================

REM Verificar uv
uv --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERRO: uv não encontrado!
    echo.
    echo Instalando uv...
    powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"
    echo.
    echo uv instalado! Por favor, reinicie o terminal e execute setup.bat novamente.
    pause
    exit /b 1
)

echo.
echo Sincronizando dependências com uv...
uv sync

echo.
echo ========================================
echo Instalação concluída!
echo ========================================
echo.
echo Para executar a aplicação:
echo   uv run python -m campelmag_gui.main
echo.
echo Ou use o comando direto:
echo   uv run campelmag-gui
echo.
echo Para ativar o ambiente virtual:
echo   .venv\Scripts\activate
echo.

pause
