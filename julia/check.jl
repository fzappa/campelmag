#!/usr/bin/env julia
"""
Script de verificação rápida - testa se o CampelMag.jl está funcionando
"""

# Ativar o projeto local
using Pkg
Pkg.activate(@__DIR__)

println("Testando CampelMag.jl...\n")

try
    include("CampelMag.jl")
    println("✓ CampelMag.jl carregado com sucesso!")
    println("✓ Epsilon = ", Epsilon)
    println("\n✓ Todas as funções estão disponíveis!")

    # Teste rápido
    println("\nTeste rápido:")
    S = 0.457
    N = 4
    result = db(S, N)
    println("  db($S, $N) = $result")

    println("\n✅ CampelMag.jl está funcionando corretamente!")
catch e
    println("❌ Erro ao carregar CampelMag.jl:")
    println(e)
    exit(1)
end
