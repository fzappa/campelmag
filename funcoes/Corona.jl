"""
...

* Calcula a tensão crítica de pico (Equação de Peek)
* CoronaVc(M,Delta, Dcond, DistCond) [kV]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Coeficiente que depende da altura e altitude
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre condutores [m]
*

...
"""
function CoronaVc(M,Delta, Dcond, DistCond)

  A0 = 2.43*M*Delta*Dcond*1000; #297.6652
  A1 = log10((2*DistCond*1000)/(Dcond*1000));

  return A0*A1;

end


"""
...

* Calcula o delta crítico
* CoronaVc(M,delta, Dcond, DistCond)
*
* H = Altitude da instalação [m]
* t = temperatura média anual [°C]
*

...
"""
function CoronaDelta(H,t)

  return (0.386*(760-0.086*H))/(273+t);

end


"""
...

* Calcula as perdas devido ao Efeito Corona
* CoronaPerdas(Delta,F,Dcond, DistCond,V,Vc) [kW/km]
*
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
* DistCond = Distância entre os condutores [m]
* V = Tensão da rede pico-a-pico [V]
* Vc = Tensão Crítica da linha [V]
*

...
"""
function CoronaPerdas(Delta,F,Dcond,DistCond,V,Vc)
  A0 = (3.44/Delta)*F;
  A1 = sqrt( (Dcond*1000) / ( 2*(DistCond*1000) ) );
  A2 = (V-Vc)*(V-Vc)*0.001;

  return A0*A1*A2;
end


"""
...

* Calcula o campo elétrico crítico
* CoronaEc(M,Delta,Dcond) [kV/cm]
*
* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
* Delta = Delta Crítico
* F = Frequência da rede [Hz]
* Dcond = Diâmetro do condutor [m]
*
*

...
"""
function CoronaEc(M,Delta,Dcond)

  A0 = M*(21.1e3)*(Delta);
  A1 = (1 + (sqrt(1/0.301)/(sqrt(Delta*Dcond*1000)) ));

  return A0*A1;

end
