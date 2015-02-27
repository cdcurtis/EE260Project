#define DEBUG_DESTRUCTOR

#include <iostream>
#include "Headers/Device.h"
#include "Headers/Schedule.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"
#include "DagGen/Headers/DagGen.h"


using namespace std;


void InitalizeDAGs(vector<DagGen*>& dags);


int main()
{
	cout << "Starting"<<endl;
	vector<DagGen*> startingPoint;
	InitalizeDAGs(startingPoint);
	DMFB board;
	Schedule* schedule = new Schedule(board);
	DynamicScheduler Scheduler;

	vector<LeveledDag*> lg;
//	startingPoint[0]->generateDotyGraph();
for(unsigned int i=0; i < startingPoint.size(); ++i)
{
	cout<<i<<endl;
	LeveledDag LDag(*startingPoint[0]);
	LDag.print();
	lg.push_back(&LDag);

}

	Scheduler.schedule(CriticalPath, schedule,lg);
	
	cout <<endl<<"Schedule:\n";
	schedule->Print();
	cout << "Ending"<<endl;

	return 0;
}


void InitalizeDAGs(vector<DagGen*>& dags)
{
	DagGen* dag = new DagGen();
	Vertex * Dis1 = dag->addVertex(DISPENSE, "D1");
	Vertex * Dis2 = dag->addVertex(DISPENSE, "D2");
	Vertex * Mix = dag->addVertex(MIX, "Mix");
	Vertex * Heat = dag->addVertex(HEAT, "Heat");
	Vertex * Output = dag->addVertex(OUTPUT, "Output");
	
	dag->addEdge(Dis1,Mix);
	dag->addEdge(Dis2,Mix);
	dag->addEdge(Mix,Heat);
	dag->addEdge(Heat,Output);	
	
	dags.push_back(dag);
/*//=============================================================================
	DagGen* dag1 = new DagGen();
	Vertex * d1_Dis1 = dag1->addVertex(DISPENSE, "D1");
	Vertex * d1_Dis2 = dag1->addVertex(DISPENSE, "D2");
	Vertex * d1_Dis3 = dag1->addVertex(DISPENSE, "D3");
	Vertex * d1_Dis4 = dag1->addVertex(DISPENSE, "D4");
	
	Vertex * d1_Mix = dag1->addVertex(MIX, "Mix");
	Vertex * d1_Mix2 = dag1->addVertex(MIX, "Mix2");
	Vertex * d1_Mix3 = dag1->addVertex(MIX, "Mix3");
	Vertex * d1_Detect = dag1->addVertex(DETECT, "Detect");
	Vertex * d1_Output = dag1->addVertex(OUTPUT, "Output");
	
	dag1->addEdge(d1_Dis1,d1_Mix);
	dag1->addEdge(d1_Dis2,d1_Mix);
	dag1->addEdge(d1_Dis3,d1_Mix2);
	dag1->addEdge(d1_Dis4,d1_Mix2);
	dag1->addEdge(d1_Mix,d1_Mix3);
	dag1->addEdge(d1_Mix2,d1_Mix3);
	dag1->addEdge(d1_Mix3,d1_Detect);
	dag1->addEdge(d1_Detect,d1_Output);	
	dags.push_back(dag1);
//=============================================================================
	DagGen* dag2 = new DagGen();
	Vertex * d2_Dis1 = dag2->addVertex(DISPENSE, "D1");
	Vertex * d2_Dis2 = dag2->addVertex(DISPENSE, "D2");
	Vertex * d2_Dis3 = dag2->addVertex(DISPENSE, "D3");
	Vertex * d2_Mix = dag2->addVertex(MIX, "Mix");
	Vertex * d2_Mix2 = dag2->addVertex(MIX, "Mix2");
	Vertex * d2_Heat = dag2->addVertex(HEAT, "Heat");
	Vertex * d2_Output = dag2->addVertex(OUTPUT, "Output");
	
	dag2->addEdge(d2_Dis1,d2_Mix);
	dag2->addEdge(d2_Dis2,d2_Mix);
	dag2->addEdge(d2_Dis3,d2_Mix2);
	dag2->addEdge(d2_Mix,d2_Mix2);
	dag2->addEdge(d2_Mix2,d2_Heat);
	dag2->addEdge(d2_Heat,d2_Output);	
	
	dags.push_back(dag2);
//=============================================================================	
	DagGen* MixTree2 = new DagGen();
		
	Vertex * mt2_Dis1 = MixTree2->addVertex(DISPENSE, "D1");
	Vertex * mt2_Dis2 = MixTree2->addVertex(DISPENSE, "D2");
	Vertex * mt2_Mix = MixTree2->addVertex(MIX, "Mix");
	Vertex * mt2_Output = MixTree2->addVertex(OUTPUT, "Output");
	
	MixTree2->addEdge(mt2_Dis1,mt2_Mix);
	MixTree2->addEdge(mt2_Dis2,mt2_Mix);
	MixTree2->addEdge(mt2_Mix,mt2_Output);
	dags.push_back(MixTree2);
	//=============================================================================	
		DagGen* MixTree4 = new DagGen();
	
	Vertex * mt4_Dis1 = MixTree4->addVertex(DISPENSE, "D1");
	Vertex * mt4_Dis2 = MixTree4->addVertex(DISPENSE, "D2");
	Vertex * mt4_Dis3 = MixTree4->addVertex(DISPENSE, "D3");
	Vertex * mt4_Dis4 = MixTree4->addVertex(DISPENSE, "D4");
	
	Vertex * mt4_Mix = MixTree4->addVertex(MIX, "Mix");
	Vertex * mt4_Mix2 = MixTree4->addVertex(MIX, "Mix2");
	Vertex * mt4_Mix3 = MixTree4->addVertex(MIX, "Mix3");
	Vertex * mt4_Output = MixTree4->addVertex(OUTPUT, "Output");
	
	MixTree4->addEdge(mt4_Dis1,mt4_Mix);
	MixTree4->addEdge(mt4_Dis2,mt4_Mix);
	MixTree4->addEdge(mt4_Dis3,mt4_Mix2);
	MixTree4->addEdge(mt4_Dis4,mt4_Mix2);
	
	MixTree4->addEdge(mt4_Mix,mt4_Mix3);
	MixTree4->addEdge(mt4_Mix2,mt4_Mix3);
	
	MixTree4->addEdge(mt4_Mix3,mt4_Output);
	dags.push_back(MixTree4);
	//=============================================================================	
	DagGen* MixTree8 = new DagGen();
	
	Vertex * mt8_Dis1 = MixTree8->addVertex(DISPENSE, "D1");
	Vertex * mt8_Dis2 = MixTree8->addVertex(DISPENSE, "D2");
	Vertex * mt8_Dis3 = MixTree8->addVertex(DISPENSE, "D3");
	Vertex * mt8_Dis4 = MixTree8->addVertex(DISPENSE, "D4");
	Vertex * mt8_Dis5 = MixTree8->addVertex(DISPENSE, "D5");
	Vertex * mt8_Dis6 = MixTree8->addVertex(DISPENSE, "D6");
	Vertex * mt8_Dis7 = MixTree8->addVertex(DISPENSE, "D7");
	Vertex * mt8_Dis8 = MixTree8->addVertex(DISPENSE, "D8");
	
	Vertex * mt8_Mix = MixTree8->addVertex(MIX, "Mix");
	Vertex * mt8_Mix2 = MixTree8->addVertex(MIX, "Mix2");
	Vertex * mt8_Mix3 = MixTree8->addVertex(MIX, "Mix3");
	Vertex * mt8_Mix4 = MixTree8->addVertex(MIX, "Mix4");
	Vertex * mt8_Mix5 = MixTree8->addVertex(MIX, "Mix5");
	Vertex * mt8_Mix6 = MixTree8->addVertex(MIX, "Mix6");
	Vertex * mt8_Mix7 = MixTree8->addVertex(MIX, "Mix7");
	
	
	Vertex * mt8_Output = MixTree8->addVertex(OUTPUT, "Output");
	
	MixTree8->addEdge(mt8_Dis1,mt8_Mix);
	MixTree8->addEdge(mt8_Dis2,mt8_Mix);
	MixTree8->addEdge(mt8_Dis3,mt8_Mix2);
	MixTree8->addEdge(mt8_Dis4,mt8_Mix2);
	MixTree8->addEdge(mt8_Dis5,mt8_Mix4);
	MixTree8->addEdge(mt8_Dis6,mt8_Mix4);
	MixTree8->addEdge(mt8_Dis7,mt8_Mix5);
	MixTree8->addEdge(mt8_Dis8,mt8_Mix5);
	
	MixTree8->addEdge(mt8_Mix,mt8_Mix3);
	MixTree8->addEdge(mt8_Mix2,mt8_Mix3);
	MixTree8->addEdge(mt8_Mix4,mt8_Mix6);
	MixTree8->addEdge(mt8_Mix5,mt8_Mix6);
	
	MixTree8->addEdge(mt8_Mix3,mt8_Mix7);
	MixTree8->addEdge(mt8_Mix6,mt8_Mix7);
	
	MixTree8->addEdge(mt8_Mix7,mt8_Output);
	dags.push_back(MixTree8);
	//=============================================================================	
*/
}

