"""
...

* Retorna um vetor com o campo magnético resultante nos pontos da faixa de passagem
* [SBr SBx SBh B] - Campo em mG (1 mili Gauss = 0.1 micro Tesla)
* Brms(I,H,D,P)
* I = Corrente que passa pela linha. [IA IB IC]
* H = Vetor com as posições verticais dos cabos. [A B C]
* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
* P = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
*

...
"""
function Brms(I,H,D,P)

  Xm = collect(P[1]:P[3]:P[2]);
  Hm = P[4];
  Bkr = complex(zeros(length(Xm),3));
  Bkx = complex(zeros(length(Xm),3));
  Bkh = complex(zeros(length(Xm),3));

  for i=1:3
    Bkr[:,i] = ((2e-7)*complex(I[i]))./sqrt(complex(Xm-D[i]).^2 + (Hm-H[i])^2);
    Bkx[:,i] = ((2e-7)*complex(I[i])*(Xm-D[i]))./sqrt(complex(Xm-D[i]).^2 + (Hm-H[i])^2);
    Bkh[:,i] = ((2e-7)*complex(I[i])*(Hm-H[i]))./sqrt(complex(Xm-D[i]).^2 + (Hm-H[i])^2);
  end

  SBr = sum(Bkr,2);
  SBx = sum(Bkx,2);
  SBh = sum(Bkh,2);
  Bmag = sqrt(real(SBx).^2 + imag(SBx).^2 + real(SBh).^2 + imag(SBh).^2);

  return [SBr*(1e5) SBx*(1e5) SBh*(1e5) Bmag*(1e5)];

end
