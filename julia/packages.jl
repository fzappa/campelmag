#!/usr/bin/julia

print_with_color(:red,"\n#### Instalando dependências ####\n\n");

print_with_color(:red,"Atualizando base...\n");
Pkg.update();

print_with_color(:red,"Instalando constante físicas...\n");
Pkg.clone("https://github.com/DataWookie/PhysicalConstants.jl")

print_with_color(:red,"Instalando PyCall (Julia)...\n");
Pkg.add("PyCall");

print_with_color(:red,"Instalando Matplotlib (Python)...\n");
run(`sudo pip install matplotlib`);

print_with_color(:red,"\n\nInstalar o pacote tk \n");
print_with_color(:red,"# pacman -S tk (Arch Linux) \n");
print_with_color(:red,"# apt install tk (Ubuntu) \n");