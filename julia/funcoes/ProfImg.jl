"""
...

* Retorna a profundidade da imagem do condutor para o campo Magnético
* [Delta] - Profundidade em metros
* ProfImg(R,P,F)
* R = Resistividade ρ do solo. [Ω]
* P = Permeabilidade μ do solo. [H/m] Típico = 4π 10^-7 H/m
* F = Frequência da rede [Hz]
*

...
"""
function ProfImg(R,P,F)
  return sqrt(R/(pi*F*P));
end
