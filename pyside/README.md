# CampelMag GUI - Interface PySide6

Interface gráfica baseada em PySide6 para cálculo de campos elétrico e magnético
em linhas de transmissão.

![CampelMag PySide6](screenshot.png)

## Descrição

Este projeto integra a lógica Python da biblioteca CampelMag com a interface Qt, permitindo:

- Cálculo de campos elétricos e magnéticos
- Visualização gráfica dos resultados
- Interface interativa com atualização em tempo real
- Configuração de parâmetros da linha de transmissão

## Requisitos

- Python 3.11 ou superior
- uv instalado (os scripts de setup instalam automaticamente caso não esteja presente)

## Instalação

### Usando uv (Recomendado) ⚡

[uv](https://github.com/astral-sh/uv) é um gerenciador de pacotes Python extremamente rápido.

#### Windows

```bash
setup.bat
```

#### Linux/macOS

```bash
bash setup.sh
```

Os scripts acima instalarão o `uv` automaticamente se não estiver presente.

#### Manual com uv

```bash
# Instalar uv (se necessário)
# Windows (PowerShell):
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"

# Linux/macOS:
curl -LsSf https://astral.sh/uv/install.sh | sh

# Sincronizar dependências
uv sync
```

### Alternativa com pip/venv

Veja [UV_GUIDE.md](UV_GUIDE.md) para instruções detalhadas sobre uv ou use pip tradicional:

```bash
python -m venv venv
source venv/bin/activate  # Linux/macOS
# ou
venv\Scripts\activate  # Windows
pip install -r requirements.txt
```

## Uso

### Com uv (Recomendado)

```bash
# Executar diretamente (sem ativar ambiente)
uv run python -m campelmag_gui.main

# Ou usar o comando registrado
uv run campelmag-gui
```

### Com ambiente virtual ativado

```bash
# Ativar ambiente
source .venv/bin/activate  # Linux/macOS
# ou
.venv\Scripts\activate  # Windows

# Executar
python -m campelmag_gui.main
# ou
campelmag-gui
```

## Estrutura do Projeto

```
pyside/
├── campelmag_gui/
│   ├── __init__.py
│   ├── main.py              # Ponto de entrada da aplicação
│   ├── main_window.py       # Lógica da janela principal
│   ├── ui_mainwindow.py     # Interface gerada do Qt Designer
│   └── campmag.py           # Biblioteca de cálculo (do projeto Python)
├── .venv/                   # Ambiente virtual (criado por uv)
├── uv.lock                  # Lockfile para builds reproduzíveis
├── requirements.txt         # Compatibilidade com pip
├── pyproject.toml           # Configuração do projeto
├── setup.bat                # Setup Windows (com uv)
├── setup.sh                 # Setup Linux/macOS (com uv)
├── UV_GUIDE.md              # Guia completo do uv
├── COMANDOS.md              # Comandos úteis
├── QUICKSTART.md            # Guia rápido
└── README.md                # Este arquivo
```

## Funcionalidades

- **Entrada de Parâmetros**: Tensão, potência, altura, flecha, espaçamento, etc.
- **Cálculo Automático**: Campos elétrico e magnético calculados em tempo real
- **Gráficos Interativos**: Visualização dos perfis de campo usando matplotlib
- **Configuração de Fases**: Posição e ângulos das fases A, B, C
- **Faixa de Medição**: Configuração do perfil de medição

## Exemplo de Uso

O programa vem com valores padrão de uma linha de 765 kV:

- Tensão: 765 kV
- Potência: 2.2 GW
- Altura máxima: 25 m
- 4 condutores por feixe

Clique em "Calcular" para visualizar os resultados.

## Documentação Adicional

- 📖 [QUICKSTART.md](QUICKSTART.md) - Início rápido
- ⚡ [UV_GUIDE.md](UV_GUIDE.md) - Guia completo do uv
- 🔧 [COMANDOS.md](COMANDOS.md) - Comandos úteis para desenvolvimento
- 🔄 [MIGRATION.md](MIGRATION.md) - Migração de venv para uv
- 📝 [exemplo.py](exemplo.py) - Exemplo de uso programático

## Licença

MIT License
