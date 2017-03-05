#include <iostream>
#include "FuncoesMag.h"
#include "Constantes.h"

using namespace std;

int main()
{
	
	try{
		
		cout << CoronaVc(0.93,1,0.493,8) << endl;
		cout << CoronaDeltaCrit(500.0, 30.0) << endl;
		cout << CoronaPerdas(0.83,60,0.057,0.457,765e3,10000) << endl;
		cout << CoronaEC(500.0,0.83,0.457) << endl;
		cout << db(0.457,4) << endl;
		cout << deq(0.457,0.032,4) << endl;
		cout << ProfImg(100,4*3.14*10e-7,60) << endl;
		
	}	catch (std::exception &e){
		cout << "Erro: " << e.what() << "\n";
	}
	
	return 0;
}
