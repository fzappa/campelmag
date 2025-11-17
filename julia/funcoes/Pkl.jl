"""
...

* Calcula o coeficiente de potencial mútuo
* entre os condutores k e l.
*   Pkl(Skll, Skl)
* Skll = Distância entre o condutor k e a
*        imagem do condutor l.
* Skl = Distância entre o condutor k e l.

...
"""
function Pkl(proprio,mutuo)
  return (1/(2*pi*Epsilon))*log.(proprio./mutuo);
end
