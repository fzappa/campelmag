#!/usr/bin/env julia
"""
Teste rápido das funções principais do CampelMag
"""

# Ativar o projeto local
using Pkg
Pkg.activate(@__DIR__)

include("CampelMag.jl")
using Printf

println("="^60)
println("Testando módulo CampelMag")
println("="^60)

# Teste 1: Funções básicas
println("\n[Teste 1] Funções geométricas básicas")
println("-" * "="^40)

S = 0.457  # Espaçamento entre subcondutores [m]
N = 4      # Número de condutores
D_cabo = 0.02959  # Diâmetro do cabo [m]

DB = db(S, N)
println(@sprintf "  db(%5.3f, %d) = %5.3f m" S N DB)

DEQ = deq(DB, D_cabo, N)
println(@sprintf "  deq(%5.3f, %5.5f, %d) = %5.5f m" DB D_cabo N DEQ)

# Teste 2: Matriz de distâncias
println("\n[Teste 2] Matrizes de distância")
println("-" * "="^40)

D = [-14.34, 0, 14.34]  # Posições horizontais [m]
H = [16.37, 16.37, 16.37]  # Alturas [m]

println("  Posições D = ", D)
println("  Alturas H = ", H)

Skl_result = Skl(D, H)
println("  Skl (distâncias entre condutores):")
for i in 1:3
    println(@sprintf "    [%6.2f %6.2f %6.2f]" Skl_result[i,1] Skl_result[i,2] Skl_result[i,3])
end

# Teste 3: Correntes e tensões
println("\n[Teste 3] Correntes e tensões")
println("-" * "="^40)

V = 765e3  # Tensão [V]
I = 1000.0  # Corrente [A]
Angulos = [120, 0, -120]  # Ângulos [graus]

V_matrix = Vri(V, Angulos)
println(@sprintf "  Tensão de linha: %6.1f kV" V/1000)
println("  Matriz de tensões (real, imag):")
for i in 1:3
    println(@sprintf "    Fase %d: [%10.2f, %10.2f] V" i V_matrix[i,1] V_matrix[i,2])
end

I_matrix = Iri(I, Angulos)
println(@sprintf "\n  Corrente: %6.1f A" I)
println("  Matriz de correntes (real, imag):")
for i in 1:3
    println(@sprintf "    Fase %d: [%8.2f, %8.2f] A" i I_matrix[i,1] I_matrix[i,2])
end

# Teste 4: Efeito Corona
println("\n[Teste 4] Efeito Corona")
println("-" * "="^40)

M = 0.87  # Coeficiente de rugosidade para cabos
H_alt = 500  # Altitude [m]
t = 25  # Temperatura [°C]
Delta = CoronaDelta(H_alt, t)
println(@sprintf "  Delta crítico (H=%d m, T=%d°C): %5.3f" H_alt t Delta)

Dist_cond = 14.34  # Distância entre condutores [m]
Vc = CoronaVc(M, Delta, D_cabo, Dist_cond)
println(@sprintf "  Tensão crítica: %6.2f kV" Vc)

Ec = CoronaEc(M, Delta, D_cabo)
println(@sprintf "  Campo elétrico crítico: %6.2f kV/cm" Ec/1000)

# Teste 5: Campo elétrico simplificado
println("\n[Teste 5] Cálculo de campo elétrico")
println("-" * "="^40)

LinhaDeMedicao = [-40, 40, 5, 1.5]  # [Xmin Xmax Passo Altura]
E_result = CalcEkv(H, D, S, N, D_cabo, V, Angulos, LinhaDeMedicao)

println(@sprintf("  Perfil: %.1f a %.1f m, passo %.1f m, altura %.1f m",
    LinhaDeMedicao[1], LinhaDeMedicao[2], LinhaDeMedicao[3], LinhaDeMedicao[4]))
println(@sprintf "  Campo elétrico máximo: %5.2f kV/m" maximum(real(E_result[:,3])))

# Teste 6: Campo magnético simplificado
println("\n[Teste 6] Cálculo de campo magnético")
println("-" * "="^40)

I_complex = [complex(I_matrix[i,1], I_matrix[i,2]) for i in 1:3]
B_result = Brms(I_complex, H, D, LinhaDeMedicao)

println(@sprintf "  Corrente por fase: %.1f A" I)
println(@sprintf "  Campo magnético máximo: %5.2f µT" maximum(real(B_result[:,4])))

println("\n" * "="^60)
println("✓ Todos os testes concluídos com sucesso!")
println("="^60)
