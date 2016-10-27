"""
...

* Calcula a distância entre os condutores k e l.
* Skl(D,H)
* D = Matriz com a posição relativa das fases.
* H = Matriz com a altura relativa das fases
* D = [-14  0  14] (exemplo)
* H = [ 28  28 28]


...
"""
function Skl(D,H)
    return [0  sqrt((D[2]-D[1])^2 + (H[2]-H[1])^2)  sqrt((D[3]-D[1])^2 + (H[3]-H[1])^2);
            sqrt((D[1]-D[2])^2 + (H[1]-H[2])^2)  0  sqrt((D[3]-D[2])^2 + (H[3]-H[2])^2);
            sqrt((D[1]-D[3])^2 + (H[1]-H[3])^2)  sqrt((D[2]-D[3])^2 + (H[2]-H[3])^2)  0]
end
