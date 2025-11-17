#!/usr/bin/env julia

using Pkg

printstyled("\n#### Instalando dependências ####\n\n"; color=:red)

printstyled("Atualizando pacotes...\n"; color=:red)
Pkg.update()

# PhysicalConstants.jl está no registro geral, não precisa mais clonar
printstyled("Instalando constantes físicas...\n"; color=:red)
Pkg.add("PhysicalConstants")

printstyled("Instalando PyCall (Julia)...\n"; color=:red)
Pkg.add("PyCall")

printstyled("Instalando PyPlot (Julia)...\n"; color=:red)
Pkg.add("PyPlot")

printstyled("Instalando Matplotlib (Python)...\n"; color=:red)
run(`pip install matplotlib`)

printstyled("\n\nInstalar o pacote tk \n"; color=:red)
printstyled("# pacman -S tk (Arch Linux) \n"; color=:red)
printstyled("# apt install tk (Ubuntu) \n"; color=:red)