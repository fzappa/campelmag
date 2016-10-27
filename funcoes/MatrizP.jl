"""
...

* Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3)
* MatrizP(Mp, Mm)
* Mp = Matriz própria de coeficientes
* Mm = Matriz mútua de coeficientes
*

...
"""
function MatrizP(Mp,Mm)
  M = Mm;
  M[1,1] = Mp[1,1];
  M[2,2] = Mp[2,2];
  M[3,3] = Mp[3,3];

  return M;
end
