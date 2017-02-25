"""
...

* Calcula o diâmetro do feixe
* db(S,N)
* S = Espaçamento entre os condutores
* N = Número de condutores

...
"""
function db(S,N)
  return S/sin(pi/N);
end
