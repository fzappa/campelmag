# CampelMag.jl

# Cálculo de Campos Elétricos e Magnéticos de Linhas de Transmissão

Implementação de funções para cálculo 2D de campos elétricos e magnéticos de linhas de transmissão aéreas.

Baseado em: **EPRI AC - Transmission Line Reference Book - 200kV and above**
Capítulo 7 - Electric and Magnetic Fields

---

## ⚡ Início Rápido

### 1️⃣ Instalar Dependências

**Execute primeiro (obrigatório):**

```bash
julia install.jl
```

### 2️⃣ Executar Exemplo

```bash
julia exemplo.jl
```

### 3️⃣ Executar Testes

```bash
julia test.jl
```

---

## Instalação### Método 1: Instalação Rápida (Recomendado)

Execute o script de instalação automática:

```bash
julia install.jl
```

Este script irá:

- Instalar todos os pacotes Julia necessários
- Verificar a instalação
- Avisar se o matplotlib (Python) não estiver instalado

### Método 2: Instalação Manual

```julia
using Pkg

# Instalar pacotes Julia
Pkg.add("PhysicalConstants")
Pkg.add("Unitful")
Pkg.add("PyCall")
Pkg.add("PyPlot")
Pkg.add("Printf")
```

### Método 3: Usando Project.toml

```bash
julia --project=. -e 'using Pkg; Pkg.instantiate()'
```

### Dependências do Sistema

É necessário ter Python com matplotlib instalado:

```bash
# Linux (Ubuntu/Debian)
sudo apt install python3-matplotlib tk

# Arch Linux
sudo pacman -S python-matplotlib tk

# Windows/macOS
pip install matplotlib
```

## Uso

### Modo Simples (compatibilidade com código antigo)

**Importante:** Sempre ative o projeto antes de usar:

```julia
using Pkg
Pkg.activate(".")

include("CampelMag.jl")
# Agora você pode usar as funções
```

Ou use os scripts prontos:

```bash
# Verificar se está funcionando
julia check.jl

# Executar exemplo completo
julia exemplo.jl

# Executar testes
julia test.jl
```

### Modo Módulo (recomendado)

```julia
include("src/CampelMag.jl")
using .CampelMag

# Usar as funções exportadas
E = CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
B = Brms(I, H, D, LinhaDeMedicao)
```

---

## 🆘 Problemas?

Se você encontrar erros como:

- `Package PhysicalConstants not found`
- `PyError` ou `matplotlib not found`
- Problemas com PyCall/PyPlot

👉 **Consulte o [Guia de Solução de Problemas](TROUBLESHOOTING.md)**

---

## Estrutura do Projeto

```
CampelMag.jl/
├── Project.toml          # Configuração do pacote
├── CampelMag.jl          # Arquivo principal (compatibilidade)
├── LT-3F-765kv.jl       # Exemplo de uso
├── packages.jl           # Script de instalação de dependências
├── src/
│   └── CampelMag.jl     # Módulo principal
└── funcoes/
    ├── Brms.jl          # Campo magnético
    ├── CalcEkv.jl       # Campo elétrico
    ├── Corona.jl        # Efeito corona
    └── ...              # Outras funções auxiliares
```

## Autor

Alan Franco Rosa
Data: 03/08/2016
Atualizado: 2025
