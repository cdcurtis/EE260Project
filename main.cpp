#define DEBUG_DESTRUCTOR

#include <iostream>
#include <cstdio>
#include "Headers/Device.h"
#include "Headers/Schedule.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"
#include "DagGen/Headers/DagGen.h"


using namespace std;


void InitalizeDAGs(vector<DagGen*>& dags, int number);


int main()
{
	cout << "Starting"<<endl;
	vector<DagGen*> startingPoint;
	InitalizeDAGs(startingPoint,256);
	DMFB board;
	Schedule* schedule = new Schedule(board);
	DynamicScheduler Scheduler;

	vector<LeveledDag*> lg;
	//	startingPoint[0]->generateDotyGraph();
	//for(unsigned int i=0; i < startingPoint.size(); ++i)
	//{
	//	cout<<i<<endl;
		cout<< "MakeLeveledDag" <<endl;
		LeveledDag* LDag= new LeveledDag(startingPoint[0],true);
		cout<< "FinishedLevelDag" <<endl;
	//	startingPoint[0]->generateDotyGraph("Test.dot");
		LDag->print();
		lg.push_back(LDag);

	//}

	Scheduler.schedule(CriticalPath, schedule,lg);

	cout <<endl<<"Schedule:\n";
	schedule->Print();
	cout << "Ending"<<endl;

	return 0;
}


void InitalizeDAGs(vector<DagGen*>& dags, int number)
{
	string PathPrefix = "Inputs\\Remia";
	string PathPosFix = "_256.dag";
	char buffer[50];
	int i=62;
	//for(int i =1; i< number; ++i) {
		sprintf(buffer,"Inputs\\Remia%i_256.dag",i);
		cout<<buffer<<endl;
		dags.push_back(new DagGen(buffer));
//	}
}

