#include "../Headers/DynamicScheduler.h"


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
int DynamicScheduler::schedule(ScheduleType type, Device* deviceType, Schedule& schedule,  std::vector<LeveledDag>& dags) {

	//initialize the scheduler.



	switch(type) {

		case FIFO:
			return scheduleFIFO(deviceType, schedule, dags);
		
		case Operations:
			return scheduleOpReady(deviceType, schedule, dags);
		
		case CriticalPath:
			return scheduleCritPath(deviceType, schedule, dags);
		
		case ResourcesNeeded:
		  return scheduleResNeed(deviceType, schedule, dags);
		
		default:
			return scheduleFIFO(deviceType, schedule, dags);
	}
	return 0;
}

int DynamicScheduler::scheduleFIFO(Device* deviceType, Schedule& schedule, std::vector<LeveledDag> & dags)
{
	return -1;
}

int DynamicScheduler::scheduleOpReady(Device* deviceType, Schedule& schedule, std::vector<LeveledDag>& dags)
{
	return -1;
}

int DynamicScheduler::scheduleCritPath(Device* deviceType, Schedule& schedule, std::vector<LeveledDag>& dags)
{
	return -1;
}

int DynamicScheduler::scheduleResNeed(Device* deviceType, Schedule& schedule, std::vector<LeveledDag>& dags)
{
	return -1;
	}
