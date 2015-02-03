#include <iostream>
#include "Device.h"

using namespace std;

int main()
{
	DMFB board;
	
	cout << board.Modules().size()<<endl;
	return 0;
}