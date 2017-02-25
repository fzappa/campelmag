# Para instalar o pacote com as constantes físicas
# Pkg.clone("https://github.com/DataWookie/PhysicalConstants.jl")

using PhysicalConstants
Epsilon = PhysicalConstants.MKS.VacuumPermittivity;



"""
...

* Calcula o coeficiente de potencial próprio
* do condutor k.
*   Pkk(H, deq)
* H  = Altura do condutor
* deq  = Diâmetro equivalente do condutor

...
"""
function Pkk(H,deq)
  Saida = zeros(3,3);
  M = (1/(2*pi*Epsilon))*log(4*H/deq);
  Saida[1,1] = M[1,1];
  Saida[2,2] = M[2,1];
  Saida[3,3] = M[3,1];
  return Saida;
end
