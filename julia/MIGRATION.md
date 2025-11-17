# Guia de Migração para Julia Moderno

Este documento descreve as mudanças feitas para atualizar o código para Julia 1.6+.

## Principais Mudanças

### 1. Sistema de Pacotes

**Antes:**

```julia
Pkg.update()
Pkg.clone("https://github.com/...")
Pkg.add("PyCall")
```

**Depois:**

```julia
using Pkg
Pkg.activate(".")
Pkg.instantiate()
```

Agora use `Project.toml` para gerenciar dependências.

### 2. Impressão com Cores

**Antes:**

```julia
print_with_color(:red, "Mensagem")
```

**Depois:**

```julia
printstyled("Mensagem"; color=:red)
```

### 3. Operações de Array

**Antes:**

```julia
sum(array, 2)  # Soma ao longo da dimensão 2
```

**Depois:**

```julia
sum(array, dims=2)  # Sintaxe com palavra-chave
```

### 4. PyCall e PyPlot

**Antes:**

```julia
@pyimport matplotlib.pyplot as plt
plt.plot(x, y)
plt.show()
```

**Depois:**

```julia
using PyPlot
plot(x, y)
PyPlot.show()
```

### 5. Shebang

**Antes:**

```julia
#!/usr/bin/julia
#!/usr/bin/julia -p2
```

**Depois:**

```julia
#!/usr/bin/env julia
```

### 6. Estrutura de Módulo

**Novo:** Estrutura de módulo moderna em `src/CampelMag.jl`

```julia
module CampelMag
    # ... código do módulo ...
    export Brms, CalcEkv, Corona, ...
end
```

### 7. PhysicalConstants (v0.2+)

**Antes (API antiga):**

```julia
using PhysicalConstants
Epsilon = PhysicalConstants.MKS.VacuumPermittivity
```

**Depois (API v0.2+):**

```julia
using PhysicalConstants.CODATA2018: ε_0
using Unitful: ustrip, @u_str

# Extrair valor numérico (sem unidades)
const Epsilon = ustrip(Float64, u"F/m", ε_0)
```

A nova versão do PhysicalConstants usa:

- API CODATA2018 para constantes físicas
- Unitful.jl para unidades físicas
- É necessário extrair o valor numérico com `ustrip()` para cálculos

## Compatibilidade

- Julia 1.6 ou superior
- PhysicalConstants.jl agora está no registro geral
- PyCall e PyPlot continuam funcionando normalmente

## Código Legado

O arquivo `CampelMag.jl` mantém compatibilidade com código antigo que usa `include()`.
Para novo código, recomenda-se usar o módulo em `src/CampelMag.jl`.

## Melhorias de Performance

As macros `@simd` e `@fastmath` continuam válidas e foram mantidas para otimização.
O broadcasting com `.` também é mantido para operações vetorizadas eficientes.

## Exemplo de Uso

### Importante: Ativar o Projeto

**Sempre ative o projeto antes de usar:**

```julia
using Pkg
Pkg.activate(".")
```

### Modo Antigo (compatível)

```julia
using Pkg
Pkg.activate(".")
include("LT-3F-765kv.jl")
```

Ou use os scripts prontos:

```bash
julia exemplo.jl      # Exemplo completo
julia test.jl         # Testes
julia check.jl        # Verificação rápida
```

### Modo Novo (recomendado)

```julia
include("src/CampelMag.jl")
using .CampelMag

E = CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
B = Brms(I, H, D, LinhaDeMedicao)
```

## Testes

Execute o exemplo atualizado:

```bash
julia exemplo.jl
```

Ou use o arquivo original (ainda compatível):

```bash
julia LT-3F-765kv.jl
```
