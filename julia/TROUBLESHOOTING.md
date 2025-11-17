# Solução de Problemas - CampelMag.jl

## ❌ Erro: "Package PhysicalConstants not found" ou "Package Unitful not found"

```
ERROR: LoadError: ArgumentError: Package PhysicalConstants not found in current path.
ERROR: LoadError: ArgumentError: Package Unitful not found in current path.
- Run `import Pkg; Pkg.add("PhysicalConstants")` to install the PhysicalConstants package.
```

### ✅ Solução 1: Usar os Scripts Prontos

Os scripts já estão configurados corretamente:

```bash
julia check.jl     # Verificar instalação
julia exemplo.jl   # Executar exemplo
julia test.jl      # Executar testes
```

### ✅ Solução 2: Ativar o Projeto Manualmente

Se você está usando `include()` diretamente, sempre ative o projeto primeiro:

```julia
using Pkg
Pkg.activate(".")  # Ativa o projeto local

include("CampelMag.jl")
# Agora funciona!
```

### ✅ Solução 3: Instalar as Dependências

Se ainda não instalou as dependências:

```bash
julia install.jl
```

**Nota:** Quando você executa `julia -e '...'` ou `julia` sem ativar o projeto,
o Julia usa o ambiente global, que pode não ter os pacotes instalados.

---

## ❌ Erro: "PyError" ou "matplotlib not found"

```
PyError: No module named 'matplotlib'
```

### ✅ Solução

Instale o matplotlib do Python:

```bash
# Linux/macOS
pip install matplotlib

# Windows
python -m pip install matplotlib

# Ubuntu/Debian
sudo apt install python3-matplotlib

# Arch Linux
sudo pacman -S python-matplotlib
```

Depois reconfigure o PyCall:

```bash
julia -e 'using Pkg; Pkg.build("PyCall")'
```

---

## ❌ Erro: "LoadError" ao usar PyPlot

```
ERROR: LoadError: InitError: PyError ...
```

### ✅ Solução

1. Verifique se o matplotlib está instalado:

   ```bash
   python -c "import matplotlib; print(matplotlib.__version__)"
   ```

2. Reconstrua PyCall:

   ```bash
   julia -e 'using Pkg; Pkg.build("PyCall")'
   ```

3. Se persistir, reconfigure o PyCall para usar um Python específico:

   ```bash
   export PYTHON="/usr/bin/python3"  # Linux/macOS
   # ou
   set PYTHON=C:\Python39\python.exe  # Windows

   julia -e 'using Pkg; Pkg.build("PyCall")'
   ```

---

## ❌ Erro: "cannot open shared object file"

```
ERROR: LoadError: InitError: could not load library "libpython3.X.so"
```

### ✅ Solução

Instale as bibliotecas de desenvolvimento do Python:

```bash
# Ubuntu/Debian
sudo apt install python3-dev

# Arch Linux
sudo pacman -S python

# macOS (via Homebrew)
brew install python3
```

Depois reconstrua PyCall:

```bash
julia -e 'using Pkg; Pkg.build("PyCall")'
```

---

## ❌ Erro: "MKS not defined in PhysicalConstants"

```
ERROR: LoadError: UndefVarError: `MKS` not defined in `PhysicalConstants`
```

### ✅ Solução

Este erro ocorre com versões antigas do código usando a nova API do PhysicalConstants v0.2+.

**O código já foi atualizado!** Se você ainda vê este erro:

1. Certifique-se de ter a versão mais recente dos arquivos
2. Recompile o pacote:
   ```bash
   julia -e 'using Pkg; Pkg.build("CampelMag")'
   ```

A mudança foi de:

```julia
# Antigo (não funciona mais)
Epsilon = PhysicalConstants.MKS.VacuumPermittivity

# Novo (correto)
using PhysicalConstants.CODATA2018: ε_0
using Unitful: ustrip, @u_str
const Epsilon = ustrip(Float64, u"F/m", ε_0)
```

---

## ❌ Erro: "DimensionError" com PhysicalConstants

```
ERROR: DimensionError:  and m F^-1 are not dimensionally compatible.
```

### ✅ Solução

Este erro ocorre quando PhysicalConstants retorna valores com unidades (Unitful),
mas o código espera números puros (Float64).

**O código já foi atualizado!** O `Epsilon` agora usa `ustrip()` para extrair
o valor numérico sem unidades.

Se você ainda vê este erro, certifique-se de ter a versão mais recente dos arquivos.

---

## ❌ Erro: "UndefVarError: Epsilon not defined"

```
ERROR: LoadError: UndefVarError: Epsilon not defined
```

### ✅ Solução

Certifique-se de que você está usando o `CampelMag.jl` corretamente:

**Opção 1:** Incluir diretamente (compatibilidade)

```julia
include("CampelMag.jl")
# Agora você pode usar as funções
```

**Opção 2:** Usar como módulo (recomendado)

```julia
include("src/CampelMag.jl")
using .CampelMag
```

---

## ❌ Versão do Julia muito antiga

```
ERROR: LoadError: syntax: invalid keyword argument syntax "color=:red"
```

### ✅ Solução

Atualize para Julia 1.6 ou superior:

```bash
# Verificar versão atual
julia --version

# Baixar versão mais recente em:
# https://julialang.org/downloads/
```

---

## ✅ Verificação Rápida do Ambiente

Execute este comando para verificar se tudo está instalado corretamente:

```julia
julia -e '
using Pkg
println("Julia: ", VERSION)
try
    using PhysicalConstants; println("✓ PhysicalConstants")
    using PyCall; println("✓ PyCall")
    using PyPlot; println("✓ PyPlot")
    using Printf; println("✓ Printf")
    println("\n✓ Todos os pacotes estão instalados!")
catch e
    println("✗ Erro: ", e)
    println("\nExecute: julia install.jl")
end
'
```

---

## 📧 Ainda com problemas?

Se nenhuma das soluções acima funcionou:

1. Verifique se você está usando Julia 1.6+: `julia --version`
2. Tente remover e reinstalar os pacotes:
   ```bash
   julia -e 'using Pkg; Pkg.rm(["PhysicalConstants", "PyCall", "PyPlot"]); Pkg.gc()'
   julia install.jl
   ```
3. Crie uma issue no repositório com:
   - Versão do Julia (`julia --version`)
   - Sistema operacional
   - Mensagem de erro completa
   - Output de `julia -e 'using Pkg; Pkg.status()'`

---

## 🔍 Comandos Úteis

```bash
# Verificar versão do Julia
julia --version

# Verificar pacotes instalados
julia -e 'using Pkg; Pkg.status()'

# Limpar cache de pacotes
julia -e 'using Pkg; Pkg.gc()'

# Atualizar todos os pacotes
julia -e 'using Pkg; Pkg.update()'

# Verificar Python usado pelo PyCall
julia -e 'using PyCall; println(PyCall.python)'
```
