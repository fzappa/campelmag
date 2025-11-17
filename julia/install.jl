#!/usr/bin/env julia
"""
Script de instalação rápida para CampelMag.jl

Execute este script para instalar todas as dependências:
    julia install.jl
"""

using Pkg

println("="^60)
println("Instalando dependências do CampelMag.jl")
println("="^60)

# Ativar o ambiente do projeto
println("\n[1/5] Ativando ambiente do projeto...")
Pkg.activate(@__DIR__)

# Instalar dependências
println("\n[2/5] Instalando pacotes Julia...")
Pkg.add("PhysicalConstants")
Pkg.add("Unitful")
Pkg.add("PyCall")
Pkg.add("PyPlot")
Pkg.add("Printf")

println("\n[3/5] Atualizando pacotes...")
Pkg.update()

# Verificar instalação
println("\n[4/5] Verificando instalação...")
try
    using PhysicalConstants
    using Unitful
    using PyCall
    using PyPlot
    using Printf
    println("  ✓ PhysicalConstants.jl")
    println("  ✓ Unitful.jl")
    println("  ✓ PyCall.jl")
    println("  ✓ PyPlot.jl")
    println("  ✓ Printf")
catch e
    println("  ✗ Erro ao carregar pacotes: ", e)
    exit(1)
end

# Verificar Python matplotlib
println("\n[5/5] Verificando matplotlib (Python)...")
try
    pyimport("matplotlib")
    println("  ✓ matplotlib disponível")
catch e
    println("  ⚠ matplotlib não encontrado")
    println("\nInstale matplotlib com:")
    println("  pip install matplotlib")
    println("ou")
    println("  python -m pip install matplotlib")
end

println("\n" * "="^60)
println("✓ Instalação concluída com sucesso!")
println("="^60)
println("\nPróximos passos:")
println("  1. Execute os testes: julia test.jl")
println("  2. Execute o exemplo: julia exemplo.jl")
println("  3. Ou use: include(\"CampelMag.jl\")")
println()
