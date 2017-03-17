"""
...

* Retorna a Matriz com as tensões(3,2)
* Vri(V, Angulos)
* V = Tensão de fase
* Angulos = Angulos de defasegem das fases. [A B C]
*

...
"""
function Vri(V,Angulos)
  return [cosd(Angulos[1])*V/sqrt(3) sind(Angulos[1])*V/sqrt(3);
          cosd(Angulos[2])*V/sqrt(3) sind(Angulos[2])*V/sqrt(3);
          cosd(Angulos[3])*V/sqrt(3) sind(Angulos[3])*V/sqrt(3);];
end
