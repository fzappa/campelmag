"""
...

* Calcula o campo elétrico medido em um perfil
* CalcEkv(H, D, Espac, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
*
* H = Altura dos condutores - [HA HB HC] [m]
* D = Distância entre os condutores - [DA DB DC] [m]
* Espac = Espaçamento entre os subcondutores [m]
* Ncond = Numero de condutores por feixe
* Dcabo = Diâmetro do cabo do feixe. [m]
* V = Tensão da linha [V]
* AngulosABC = Defasagem angular entre as fases - [AA AB AC] [graus]
* LinhaDeMedicao = Perfil de medição - [Xmin Xmax Passo Altura] [m]  
*

...
"""
function CalcEkv(H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao)
  DB = db(EspacSubCond, Ncond)
  DEQ = deq(DB,Dcabo,Ncond)

  Mp = Pkk(H,DEQ);  #Matriz própria
  Mm = Pkl(Skll(D,H),Skl(D,H)); # Matriz mútua

  P = MatrizP(Mp,Mm);
  C = inv(P);
  V = Vri(V,AngulosABC);
  #Vr = V[:,1];
  #Vi = V[:,2];

  Q = C*V;

  return Erms(LinhaDeMedicao,H,D,Q)/1000;
end
