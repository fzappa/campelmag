"""
...

* Calcula a distância entre o condutor k e a
* imagem do condutor l.
* Skll(D,H)
* D = Matriz com a posição relativa das fases.
*
* D = [-14  0  14] (exemplo)
* H = [ 28  28 28]


...
"""
function Skll(D,H)
    return [2*H[1]  sqrt( (D[2]-D[1])^2 + (2*H[2])^2)  sqrt( (D[3]-D[1])^2 + (2*H[3])^2);
            sqrt( (D[1]-D[2])^2 + (2*H[1])^2)  2*H[2]  sqrt( (D[3]-D[2])^2 + (2*H[3])^2);
            sqrt( (D[1]-D[3])^2 + (2*H[1])^2)  sqrt( (D[2]-D[3])^2 + (2*H[2])^2)  2*H[3] ];
end
