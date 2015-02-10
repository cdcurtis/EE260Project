#include <iostream>
#include "Headers/Device.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"
#include "DagGen/Headers/DagGen.h"


using namespace std;

void InitalizeDAGs(DagGen* dag)
{
	Vertex * Dis1 = dag->addVertex(DISPENSE, "D1");
	Vertex * Dis2 = dag->addVertex(DISPENSE, "D2");
	Vertex * Mix = dag->addVertex(MIX, "Mix");
	Vertex * Heat = dag->addVertex(HEAT, "Heat");
	Vertex * Output = dag->addVertex(OUTPUT, "Output");
	
	dag->addEdge(Dis1,Mix);
	dag->addEdge(Dis2,Mix);
	dag->addEdge(Mix,Heat);
	dag->addEdge(Heat,Output);	
}


int main()
{
	DagGen* startingPoint = new DagGen();
	InitalizeDAGs(startingPoint);
	
	startingPoint->generateDotyGraph();
	
	
	LeveledDag LDag(*startingPoint);
	
	LDag.print();
	
	
	cout<< LDag.Levels()[3][0]->label<<endl;
	DMFB board;
	
	cout << board.Modules().size()<<endl;
	return 0;
}
