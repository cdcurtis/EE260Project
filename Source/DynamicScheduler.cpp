#include "../Headers/DynamicScheduler.h"
#include<cmath>

using namespace std;
DynamicScheduler::DynamicScheduler() {
	// Default
}

DynamicScheduler::DynamicScheduler(std:: vector<DagGen> dgAssays) {
	// Initialize DynamicScheduler Object
	// intt LeveledDAGs
}

DynamicScheduler::~DynamicScheduler() {
	// Destructor
	//	std:: cout << "DynamicScheduler destructor called.\n";
}

/* ==========================================================================
	Master schedule caller

	@params		ScheduleType
	@returns		int				status code
	========================================================================== */
int DynamicScheduler::schedule(ScheduleType type, Schedule* schedule,  std::vector<LeveledDag*>& dags) {



	switch(type) {

	case FIFO:
		selectedScheduleType = FIFO;
		return scheduleFIFO(schedule, dags);

	case Operations:
		selectedScheduleType = Operations;
		return scheduleOpReady(schedule, dags);

	case CriticalPath:
		selectedScheduleType = CriticalPath;
		return scheduleCritPath(schedule, dags);

	case ResourcesNeeded:
		selectedScheduleType = ResourcesNeeded;
		return scheduleResNeed(schedule, dags);

	default:
		return scheduleFIFO(schedule, dags);
	}
	return 0;
}
int DynamicScheduler::scheduleCritPath(Schedule* schedule, std::vector<LeveledDag*>& dags)
{
	//Order the Dags by critical path. Largest first.

	CalculateCritcalPaths(schedule,dags);
	while(dags.size() != 0)
	{
		vector<LeveledDag*>::iterator critPathDag = dags.begin();

		for(vector<LeveledDag*>::iterator temp = dags.begin(); temp != dags.end(); ++temp)
		{
			if((*critPathDag)->CriticalPathSize() < (*temp)->CriticalPathSize())
				critPathDag = temp;
		}
		//schedule->Print();
		//		cout<<(*critPathDag)->Name()<<endl;
		schedule->ScheduledDags.push_back(*critPathDag);

		ScheduleDag(schedule, *critPathDag);
		dags.erase(critPathDag);
	}

	return 0;
}
int DynamicScheduler::scheduleFIFO(Schedule* schedule, std::vector<LeveledDag*> & dags)
{
	for(vector<LeveledDag*>::iterator temp = dags.begin(); temp != dags.end();)
	{
		ScheduleDag(schedule, *temp);
		dags.erase(temp);
	}
}

int DynamicScheduler::scheduleOpReady(Schedule* schedule, std::vector<LeveledDag*>& dags)
{
	return -1;
}

void DynamicScheduler:: ScheduleDag(Schedule* schedule, LeveledDag* dag, int startingAt)
{
	dag->StartingAt() = startingAt;
	//		dag->print();
	for(unsigned int level =0 ; level < dag->Levels().size(); ++level) {
		for (unsigned int nodeIndex=0; nodeIndex <dag->Levels().at(level).size(); ++nodeIndex ) {
			ScheduleNode* node = dag->Levels().at(level).at(nodeIndex);

			if( ! schedule->PutNodeInSchdeule(node,dag,startingAt)){ // Dag Failed to Schdule
				RollBackDag(schedule,dag,node->timeEnded - node->timeStarted);
				return ScheduleDag(schedule, dag, dag->StartingAt());
			}
		}
	}
}
void DynamicScheduler:: RipUpNode(Schedule* schedule, ScheduleNode* OP)
{
	//delete any stores that came to this node.
	for(unsigned int parentIndex = 0;  parentIndex < OP->parents.size(); ++parentIndex)
	{
		if (OP->parents[parentIndex]->type == STORE){
			RipUpNode(schedule,OP->parents[parentIndex]);
			ScheduleNode * deleteME = OP->parents[parentIndex];
			OP->parents.push_back(deleteME->parents[0]);//restore the original parent that was stored.
			OP->parents.erase(OP->parents.begin()+parentIndex);

			//reassign the real child back.
			for(unsigned int childIndex = 0; childIndex < deleteME->parents[0]->children.size(); ++childIndex)
			{
				if(deleteME->parents[0]->children[childIndex] == deleteME)
				{
					deleteME->parents[0]->children.erase(deleteME->parents[0]->children.begin() + childIndex);
					deleteME->parents[0]->children.push_back(OP);
				}
			}
			delete deleteME;
			deleteME = NULL;
		}

	}

	schedule->RemoveScheduleNode(OP);
}

void DynamicScheduler :: RollBackDag(Schedule* schedule, LeveledDag* dag, int duration)
{
	//schedule->Print();
	//schedule->PrintRes();
	dag->StartingAt() +=duration;

	for(unsigned int level =0 ; level < dag->Levels().size(); ++level)
		for(unsigned int nodeIndex = 0 ; nodeIndex < dag->Levels().at(level).size(); ++nodeIndex ){
			RipUpNode(schedule, dag->Levels().at(level).at(nodeIndex));
		}

	//find if the DAG needs to be condensed.
	/*	bool allSameDag= true;

	for(unsigned int i = 0; i< schedule->schduledNodes[startingAt].size(); ++i) {
		if(dag->FindLevel(schedule->schduledNodes[startingAt].at(i)) == -1 && schedule->schduledNodes[startingAt].at(i)->type !=STORE) {
			allSameDag = false;
		}
	}
	if(allSameDag) {

		cerr << "UNSCHEDULEABLE"<<endl;
		dag->print();
		return;
	}*/

	return ;
}


void DynamicScheduler::CalculateCritcalPaths(Schedule* schedule, std::vector<LeveledDag*>& dags)
{
	for(unsigned int i = 0 ; i <dags.size(); ++i)
	{
		double critcalPathSize =0 ;
		for(unsigned int level=0; level < dags[i]->Levels().size(); ++level)
		{
			double maxAtLevel =0;
			for(unsigned nodeIndex =0; nodeIndex < dags[i]->Levels().at(level).size(); ++nodeIndex )
			{
				ScheduleNode* n= dags[i]->Levels().at(level).at(nodeIndex);
				double nodeSize = max(n->timeNeeded, schedule->GetOperationTime(n->type));

				if(maxAtLevel<nodeSize)
					maxAtLevel = nodeSize;
			}
			critcalPathSize += maxAtLevel;
		}
		dags[i]->CriticalPathSize() = critcalPathSize;
	}
}

int DynamicScheduler::scheduleResNeed(Schedule* schedule, std::vector<LeveledDag*>& dags)
{
	return -1;
}
