import numpy as np
from scipy.constants import pi, epsilon_0



class CampelMag(object):

	"""
	...
	*
	* Funções para plotar o perfil de campo elétrico e magnético
	* EPRI AC - Transmission Line Reference Book - 200kV and above
	* Capítulo 7 - Electric and Magnetic Fields
	* Autor: Alan Franco
	* 03/08/2016
	*
	...
	"""

	def __init__(self):
		self.__r3 = 1.7320508075688772  # raiz(3)
		self.__zero = 0



	def Vri(self,V,Angulos):
		"""
		...
		*
		* Retorna a Matriz com as tensões(3,2)
		* Vri(V, Angulos)
		* V = Tensão de fase [A, B, C]
		* Angulos = Angulos de defasegem das fases. [A, B, C]
		*
		...
		"""
		saida = np.zeros((3,2))
		saida[0,:] = np.cos(np.deg2rad(Angulos[0]))*V/self.__r3, np.sin(np.deg2rad(Angulos[0]))*V/self.__r3
		saida[1,:] = np.cos(np.deg2rad(Angulos[1]))*V/self.__r3, np.sin(np.deg2rad(Angulos[1]))*V/self.__r3
		saida[2,:] = np.cos(np.deg2rad(Angulos[2]))*V/self.__r3, np.sin(np.deg2rad(Angulos[2]))*V/self.__r3

		return saida



	def Skll(self,D,H):
		"""
		...
		*
		* Calcula a distância entre o condutor k e a (3,3)
		* imagem do condutor l.
		* Skll(D,H)
		* D = Matriz com a posição relativa das fases.
		*
		* D = [-14  0  14] (exemplo)
		* H = [ 28  28 28]
		...
		"""
		saida = np.zeros((3,3))
		saida[0,:] = 2*H[0], np.sqrt( (D[1]-D[0])**2 + (2*H[1])**2), np.sqrt( (D[2]-D[0])**2 + (2*H[2])**2)
		saida[1,:] = np.sqrt( (D[0]-D[1])**2 + (2*H[0])**2), 2*H[1], np.sqrt( (D[2]-D[1])**2 + (2*H[2])**2)
		saida[2,:] = np.sqrt( (D[0]-D[2])**2 + (2*H[0])**2), np.sqrt( (D[1]-D[2])**2 + (2*H[1])**2), 2*H[2]

		return saida




	def Skl(self,D,H):
		"""
		...
		*
		* Calcula a distância entre os condutores k e l. (3,3)
		* Skl(D,H)
		* D = Matriz com a posição relativa das fases.
		* H = Matriz com a altura relativa das fases
		* D = [-14  0  14] (exemplo)
		* H = [ 28  28 28]
		...
		"""

		saida = np.zeros((3,3))
		saida[0,:] = self.__zero, np.sqrt((D[1]-D[0])**2 + (H[1]-H[0])**2), np.sqrt((D[2]-D[0])**2 + (H[2]-H[0])**2)
		saida[1,:] = np.sqrt((D[0]-D[1])**2 + (H[0]-H[1])**2), self.__zero, np.sqrt((D[2]-D[1])**2 + (H[2]-H[1])**2)
		saida[2,:] = np.sqrt((D[0]-D[2])**2 + (H[0]-H[2])**2), np.sqrt((D[1]-D[2])**2 + (H[1]-H[2])**2), self.__zero

		return saida
		
		
		
	def Pkl(self,proprio,mutuo):
		"""
		...
		*
		* Calcula o coeficiente de potencial mútuo
		* entre os condutores k e l (3,3)
		*   Pkl(Skll, Skl)
		* Skll = Distância entre o condutor k e a
		*        imagem do condutor l.
		* Skl = Distância entre o condutor k e l.
		...
		"""
		return (1.0/(2*pi*epsilon_0))*np.log(proprio/mutuo)



	def Pkk(self,H,deq):
		"""
		...
		*
		* Calcula o coeficiente de potencial próprio
		* do condutor k.
		*   Pkk(H, deq)
		* H  = Altura do condutor
		* deq  = Diâmetro equivalente do condutor
		...
		"""
		
		return (1.0/(2.0*pi*epsilon_0))*np.log(np.multiply(4.0,H)/deq)




	def Iri(self,I,Angulos):
		"""
		...
		*
		* Retorna a Matriz com as correntes(2,3)[real, imag]
		* Iri(I, Angulos)
		* I = Corrente por fase (ckt equlibrado)
		* Angulos = Angulos de defasagem das fases. [A B C]
		*
		...
		"""
		saida = np.zeros((3,1),dtype=np.complex)
		saida[0] = np.complex(np.cos(np.deg2rad(Angulos[0]))*I/self.__r3, np.sin(np.deg2rad(Angulos[0]))*I/self.__r3)
		saida[1] = np.complex(np.cos(np.deg2rad(Angulos[1]))*I/self.__r3, np.sin(np.deg2rad(Angulos[1]))*I/self.__r3)
		saida[2] = np.complex(np.cos(np.deg2rad(Angulos[2]))*I/self.__r3, np.sin(np.deg2rad(Angulos[2]))*I/self.__r3)

		return saida



	def deq(self,db, D, N):
		"""
		...
		*
		* Calcula o feixe equivalente
		*   deq(db, D, N)
		* db = Diâmetro do feixe
		* D  = Diâmetro do cabo
		* N  = Número de condutores
		...
		"""
		return db*((N*D/db)**(1/N))
	


	def db(self,S,N):
		"""
		...
		*
		* Calcula o diâmetro do feixe
		* db(S,N)
		* S = Espaçamento entre os condutores
		* N = Número de condutores
		...
		"""
		return S/np.sin(pi/N)




	def ProfImg(self,R,P,F):
		"""
		...
		*
		* Retorna a profundidade da imagem do condutor para o campo Magnético
		* [Delta] - Profundidade em metros
		* ProfImg(R,P,F)
		* R = Resistividade ρ do solo. [Ω]
		* P = Permeabilidade μ do solo. [H/m] Típico = 4π 10^-7 H/m
		* F = Frequência da rede [Hz]
		*
		...
		"""
		return np.sqrt(R/(pi*F*P))




	def MatrizP(self,Mp,Mm):
		"""
		...
		*
		* Retorna a Matriz de Coeficientes de Potenciais de Maxwell (3,3)
		* MatrizP(Mp, Mm)
		* Mp = Matriz própria de coeficientes
		* Mm = Matriz mútua de coeficientes
		*
		...
		"""
		M = Mm
		M[0,0] = Mp[0]
		M[1,1] = Mp[1]
		M[2,2] = Mp[2]
		
		return M




	def CoronaVc(self,M,Delta, Dcond, DistCond):
		"""
		...
		*
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
		A0 = 2.43*M*Delta*Dcond*1000  #297.6652
		A1 = np.log10((2*DistCond*1000)/(Dcond*1000))

		return A0*A1



	def CoronaDelta(self,H,t):
		"""
		...
		*
		* Calcula o delta crítico
		* CoronaVc(M,delta, Dcond, DistCond)
		*
		* H = Altitude da instalação [m]
		* t = temperatura média anual [°C]
		*
		...
		"""
		return (0.386*(760-0.086*H))/(273+t)




	def CoronaPerdas(self,Delta,F,Dcond,DistCond,V,Vc):
		"""
		...
		*
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
		A0 = (3.44/Delta)*F
		A1 = np.sqrt( (Dcond*1000) / ( 2*(DistCond*1000) ) )
		A2 = (V-Vc)*(V-Vc)*0.001

		return A0*A1*A2




	def CoronaEc(self,M,Delta,Dcond):
		"""
		...
		*
		* Calcula o campo elétrico crítico
		* CoronaEc(M,Delta,Dcond) [kV/cm]
		*
		* M = Coeficiente de rugosidade (0,93 para fios e 0,87 para cabos)
		* Delta = Delta Crítico
		* F = Frequência da rede [Hz]
		* Dcond = Diâmetro do condutor [m]
		*
		*
		...
		"""
		A0 = M*(21.1e3)*Delta
		A1 = (1 + (np.sqrt(1/0.301)/(np.sqrt(Delta*Dcond*1000)) ))
		return A0*A1





	def CalcQ(self,H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC):
		"""
		...
		* Calcula a matriz de distribuição de cargas
		* CalcEkv(H, D, Espac, Ncond, Dcabo, V, AngulosABC)
		*
		* H = Altura dos condutores - [HA HB HC] [m]
		* D = Distância entre os condutores - [DA DB DC] [m]
		* Espac = Espaçamento entre os subcondutores [m]
		* Ncond = Numero de condutores por feixe
		* Dcabo = Diâmetro do cabo do feixe. [m]
		* V = Tensão da linha [V]
		* AngulosABC = Defasagem angular entre as fases - [AA AB AC] [graus]  
		*
		...
		"""
		DB = self.db(EspacSubCond, Ncond)
		DEQ = self.deq(DB,Dcabo,Ncond)

		Mp = self.Pkk(H,DEQ)  #Matriz própria
		Mm = self.Pkl( self.Skll(D,H), self.Skl(D,H)) # Matriz mútua

		P = self.MatrizP(Mp,Mm)
		C = np.linalg.inv(P)
		V = self.Vri(V,AngulosABC)

		return np.dot(C,V) # Q = C @ V





	def Erms(self,LinhaDeMed,H,D,Q):
		"""
		...
		* Retorna um vetor com os campos eletricos correspondentes aos pontos [SEx SEy Erms];
		* Erms(P,H,D,Q)
		* LinhaDeMed = Vetor com as caracteristicas da linha de medição [-X X Passo Altura]
		* H = Vetor com as posições verticais dos cabos. [A B C]
		* D = Vetor com as posições horizontais relativas dos cabos [A 0 C]
		* Q = Matriz com as cargas
		*
		...
		"""
		Px = np.arange(LinhaDeMed[0],LinhaDeMed[1]+LinhaDeMed[2],LinhaDeMed[2])
		Ex = np.zeros((Px.size,3),dtype=np.complex)
		Ey = np.zeros((Px.size,3),dtype=np.complex)
		Hm = LinhaDeMed[3]  #Altura do ponto de medição
		
		for i in range(3):
			Xm = Px - D[i]  #Distância entre o ponto de medição e a linha
			Hk = H[i]		 #Altura da linha

			C = np.complex(Q[i,0],Q[i,1])/(2*pi*epsilon_0) # constante
			T1x = Xm/(Xm**2 + (Hk-Hm)**2)
			T2x = Xm/(Xm**2 + (Hk+Hm)**2)

			T1y = (Hm-Hk)/(Xm**2 + (Hk-Hm)**2)
			T2y = (Hm+Hk)/(Xm**2 + (Hk+Hm)**2)

			Ex[:,i] = C*(T1x-T2x)
			Ey[:,i] = C*(T1y-T2y)

		#Soma as contribuições de cada Linha
		saida = np.zeros((Px.size,3))
		saida[:,0] = np.real(np.sum(Ex,1)) #SEx
		saida[:,1] = np.real(np.sum(Ey,1)) #SEy
		saida[:,2] = np.real(np.sqrt(np.real(saida[:,0])**2 + np.imag(saida[:,0])**2 + np.real(saida[:,1])**2 + np.imag(saida[:,1])**2)) #Erms

		return saida






	def CalcEkv(self,H, D, EspacSubCond, Ncond, Dcabo, V, AngulosABC, LinhaDeMedicao):
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
		DB = self.db(EspacSubCond, Ncond)
		DEQ = self.deq(DB,Dcabo,Ncond)

		Mp = self.Pkk(H,DEQ)  #Matriz própria
		Mm = self.Pkl( self.Skll(D,H), self.Skl(D,H)) # Matriz mútua

		P = self.MatrizP(Mp,Mm)
		C = np.linalg.inv(P)
		V = self.Vri(V,AngulosABC)

		Q = np.dot(C,V) # C @ V
		saida = self.Erms(LinhaDeMedicao,H,D,Q)

		return saida/1e3




	def Brms(self,I,H,D,P):
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
		Xm = np.arange(P[0],P[1]+P[2],P[2])
		Hm = P[3]
		Bkr = np.zeros((Xm.size,3),dtype=np.complex)
		Bkx = np.zeros((Xm.size,3),dtype=np.complex)
		Bkh = np.zeros((Xm.size,3),dtype=np.complex)

		for i in range(3):
			# Resultado em Tesla
			Bkr[:,i] = ((2e-7)*I[i])/np.sqrt((Xm-D[i])**2 + (Hm-H[i])**2)
			Bkx[:,i] = ((2e-7)*I[i]*(Xm-D[i]))/np.sqrt((Xm-D[i])**2 + (Hm-H[i])**2)
			Bkh[:,i] = ((2e-7)*I[i]*(Hm-H[i]))/np.sqrt((Xm-D[i])**2 + (Hm-H[i])**2)
		
		SBr = np.sum(Bkr,1)
		SBx = np.sum(Bkx,1)
		SBh = np.sum(Bkh,1)
		Bmag = np.sqrt(np.real(SBx)**2 + np.real(SBh)**2 )
		#Bmag = sqrt(real(SBx).^2 + imag(SBx).^2 + real(SBh).^2 + imag(SBh).^2);

		# Converte para micro Tesla (1e6)
		saida = np.zeros((Xm.size,4))

		saida[:,0] = np.real(SBr)
		saida[:,1] = np.real(SBx)
		saida[:,2] = np.real(SBh)
		saida[:,3] = np.real(Bmag)

		return saida*1e6
