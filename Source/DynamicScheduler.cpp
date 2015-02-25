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
	std:: cout << "DynamicScheduler destructor called.\n";
}

/* ==========================================================================
	Master schedule caller
	
	@params		ScheduleType
	@returns		int				status code
	========================================================================== */
int DynamicScheduler::schedule(ScheduleType type, Schedule& schedule,  std::vector<LeveledDag>& dags) {



	switch(type) {

		case FIFO:
			return scheduleFIFO(schedule, dags);
		
		case Operations:
			return scheduleOpReady(schedule, dags);
		
		case CriticalPath:
			return scheduleCritPath(schedule, dags);
		
		case ResourcesNeeded:
		  return scheduleResNeed(schedule, dags);
		
		default:
			return scheduleFIFO(schedule, dags);
	}
	return 0;
}

int DynamicScheduler::scheduleFIFO(Schedule& schedule, std::vector<LeveledDag> & dags)
{
	return -1;
}

int DynamicScheduler::scheduleOpReady(Schedule& schedule, std::vector<LeveledDag>& dags)
{
	return -1;
}

int DynamicScheduler::scheduleCritPath(Schedule& schedule, std::vector<LeveledDag>& dags)
{
	//Order the Dags by critical path. Largest first.
	CalculateCritcalPaths(schedule,dags);
	while(dags.size() != 0)
	{
		for(unsigned int i =0; i< dags.size(); ++i)
		{
			//TODO:: pick largest crit path and schedule then delete dag from list.
		}
	}

	return -1;
}
void DynamicScheduler::CalculateCritcalPaths(Schedule& schedule, std::vector<LeveledDag>& dags)
{
	for(unsigned int i = 0 ; i <dags.size(); ++i)
	{
		double critcalPathSize =0 ;
		for(unsigned int level=0; level < dags[i].Levels().size(); ++level)
		{
			double maxAtLevel =0;
			for(unsigned nodeIndex =0; nodeIndex < dags[i].Levels().at(level).size(); ++nodeIndex )
			{
				ScheduleNode n= dags[i].Levels[i]().at(level).at(nodeIndex);
				double nodeSize = max(n.timeNeeded, schedule.GetOperationTime(n.type));

				if(maxAtLevel<nodeSize)
					maxAtLevel = nodeSize;
			}
			critcalPathSize += maxAtLevel;
		}
		dags[i].CriticalPathSize() = critcalPathSize;
	}
}

int DynamicScheduler::scheduleResNeed(Schedule& schedule, std::vector<LeveledDag>& dags)
{
	return -1;
	}
