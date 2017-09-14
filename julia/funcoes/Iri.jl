"""
...

* Retorna a Matriz com as correntes(2,3)
* Iri(I, Angulos)
* I = Corrente por fase (ckt equlibrado)
* Angulos = Angulos de defasagem das fases. [A B C]
*

...
"""
function Iri(I,Angulos)
  return [cosd(Angulos[1])*I/sqrt.(3) sind(Angulos[1])*I/sqrt.(3);
          cosd(Angulos[2])*I/sqrt.(3) sind(Angulos[2])*I/sqrt.(3);
          cosd(Angulos[3])*I/sqrt.(3) sind(Angulos[3])*I/sqrt.(3);];
end

