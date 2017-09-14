"""
...

* Retorna um vetor com os campos eletricos correspondentes aos pontos [SEx SEy Erms];
* Erms(P,H,D,Q)
* P = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
* H = Vetor com as posições verticais dos cabos. [A B C]
* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
* Q = Matriz com as cargas
*

...
"""
function Erms(P,H,D,Q)

    Px = collect(P[1]:P[3]:P[2]);
    Ex = complex(zeros(length(Px),3));
    Ey = complex(zeros(length(Px),3));

    Hm = P[4];  #Altura do ponto de medição

    @simd for i=1:3
      Xm = Px - D[i]; #Distância entre o ponto de medição e a linha
      Hk = H[i]; #Altura da linha

      C = complex(Q[i,1],Q[i,2])/(2*pi*Epsilon) # constante
      @fastmath @inbounds T1x = Xm./(Xm.^2 + (Hk-Hm)^2);
      @fastmath @inbounds T2x = Xm./(Xm.^2 + (Hk+Hm)^2);

      @fastmath @inbounds T1y = (Hm-Hk)./(Xm.^2 + (Hk-Hm)^2);
      @fastmath @inbounds T2y = (Hm+Hk)./(Xm.^2 + (Hk+Hm)^2);

      @fastmath @inbounds Ex[:,i] = C*(T1x-T2x);
      @fastmath @inbounds Ey[:,i] = C*(T1y-T2y);
    end

    #Soma as contribuições de cada Linha
    SEx = sum(Ex,2);
    SEy = sum(Ey,2);
    Erms = sqrt.(real(SEx).^2 + imag(SEx).^2 + real(SEy).^2 + imag(SEy).^2);

    return [SEx SEy Erms];
end
