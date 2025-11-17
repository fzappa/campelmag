# Electric and Magnetic Fields Calculation of Transmission Lines

Implementações de funções para cálculo 2D de Campos Elétricos e Magnéticos de linhas de transmissão aéreas.

## Descrição

Este projeto implementa funções para calcular perfis de campo elétrico e magnético baseado no:

- **EPRI AC - Transmission Line Reference Book - 200kV and above**
- **Capítulo 7 - Electric and Magnetic Fields**

Autor: Alan Franco Rosa (2016)

## Instalação

### Usando uv (Recomendado - Muito Mais Rápido! 🚀)

[uv](https://github.com/astral-sh/uv) é um gerenciador de pacotes Python extremamente rápido escrito em Rust.

```bash
# Instalar uv (se ainda não tiver)
curl -LsSf https://astral.sh/uv/install.sh | sh

# Criar ambiente virtual e instalar dependências
uv venv
source .venv/bin/activate  # No Windows: .venv\Scripts\activate
uv pip install -e .

# Ou instalar apenas as dependências
uv pip install -r requirements.txt
```

### Usando pip tradicional

```bash
# Criar ambiente virtual
python -m venv .venv
source .venv/bin/activate  # No Windows: .venv\Scripts\activate

# Instalar dependências
pip install -r requirements.txt

# Ou instalar como pacote
pip install -e .
```

## Requisitos

- Python >= 3.12
- NumPy >= 1.26.0
- SciPy >= 1.11.0
- Matplotlib >= 3.8.0

## Uso

### Exemplo Básico

```python
from campelmag import CampelMag
import numpy as np

cm = CampelMag()

# Configurações da linha
V = 765e3  # Tensão (V)
H = [16.37, 16.37, 16.37]  # Altura dos condutores (m)
D = [-14.34, 0, 14.34]  # Distância entre condutores (m)
AngulosABC = [120, 0, -120]  # Ângulos das fases (graus)

# Cálculo do campo elétrico
LinhaDeMedicao = [-80, 80, 0.5, 1.5]  # [Xmin, Xmax, Passo, Altura]
E = cm.CalcEkv(H, D, 0.457, 4.0, 0.02959, V, AngulosABC, LinhaDeMedicao)
```

### Executando o exemplo

```bash
python LT-3F-765kv.py
```

## Estrutura do Projeto

```
.
├── campelmag/
│   ├── __init__.py
│   └── campmag.py          # Classe principal CampelMag
├── LT-3F-765kv.py          # Exemplo de uso
├── requirements.txt        # Dependências do projeto
├── pyproject.toml          # Configuração do projeto
├── README.md               # Este arquivo
└── LICENSE
```

## Modernizações (2025)

O código foi modernizado para Python 3.12+ com **uv**:

- ✅ **Type hints** em todos os métodos
- ✅ **f-strings** para formatação de strings
- ✅ **Pathlib** para manipulação de caminhos
- ✅ **Docstrings** em formato Google
- ✅ **Type annotations** com `numpy.typing`
- ✅ **pyproject.toml** para configuração moderna (PEP 621)
- ✅ **uv** para gerenciamento ultra-rápido de dependências
- ✅ **hatchling** como build backend moderno
- ✅ **ruff** para linting rápido
- ✅ **Remoção de código comentado** e legado
- ✅ **Melhor estrutura** de pacote Python

### Ferramentas de Desenvolvimento

```bash
# Instalar dependências de desenvolvimento com uv
uv pip install -e ".[dev]"

# Formatar código com black
black .

# Linting com ruff (muito mais rápido que flake8/pylint)
ruff check .

# Type checking com mypy
mypy campelmag/
```

## Funcionalidades

A classe `CampelMag` oferece métodos para:

- Cálculo de campos elétricos em linhas de transmissão
- Cálculo de campos magnéticos
- Análise de efeito corona
- Cálculo de coeficientes de potencial
- Geração de perfis de medição

## Licença

Veja o arquivo LICENSE para mais detalhes.
