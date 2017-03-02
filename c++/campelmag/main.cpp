#include <iostream>
#include "FuncoesMag.h"

using namespace std;

int main()
{
	
	cout << CoronaVc(0.93,1,0.493,8) << endl;
	cout << CoronaDeltaCrit(500.0, 30.0) << endl;
	cout << CoronaPerdas(0.83,60,0.057,0.457,765e3,10000) << endl;
	cout << CoronaEC(500.0,0.83,0.457) << endl;
	return 0;
}
