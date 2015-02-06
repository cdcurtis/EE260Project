#include <iostream>
#include "Headers/Device.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"

using namespace std;

int main()
{
	DMFB board;
	
	cout << board.Modules().size()<<endl;
	return 0;
}
