"""
...

* Calcula o feixe equivalente
*   deq(db, D, N)
* db = Diâmetro do feixe
* D  = Diâmetro do cabo
* N  = Número de condutores

...
"""
function deq(db, D, N)
  t = db*((N*D/db)^(1/N));
  return t;
end
