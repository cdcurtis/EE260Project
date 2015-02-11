#include "../Headers/DynamicScheduler.h"

/* ==========================================================================
	Default Constructor
	
	@params
	@returns
	========================================================================== */
DynamicScheduler::DynamicScheduler() {
	// Default
}

/* ==========================================================================
	Constructer with vector of DagGens passed in 
	
	@params
	@returns
	========================================================================== */
DynamicScheduler::DynamicScheduler(std:: vector<DagGen> dgAssays) {
	// Initialize DynamicScheduler Object
	// intt LeveledDAGs
}

/* ==========================================================================
	Destructor
	
	@params
	@returns
	========================================================================== */
DynamicScheduler::~DynamicScheduler() {
	// Destructor
	std:: cout << "DynamicScheduler destructor called.\n";
}

/* ==========================================================================
	Master schedule caller
	
	@params		ScheduleType
	@returns		int				status code
	========================================================================== */
int DynamicScheduler::schedule(ScheduleType type, Device deviceType, Schedule& schedule) {
	switch(type) {

		case FIFO:
			return scheduleFIFO(deviceType, schedule);
		
		case Operations:
			return scheduleOpReady(deviceType, schedule);
		
		case CriticalPath:
			return scheduleCritPath(deviceType, schedule);
		
		case ResourcesNeeded:
		  return scheduleResNeed(deviceType, schedule);
		
		default:
			return scheduleFIFO(deviceType, schedule);
	}
	return 0;
}

int DynamicScheduler::scheduleFIFO(Device deviceType, Schedule& schedule) 
{
	return -1;
}

int DynamicScheduler::scheduleOpReady(Device deviceType, Schedule& schedule) 
{
	return -1;
}

int DynamicScheduler::scheduleCritPath(Device deviceType, Schedule& schedule) 
{
	return -1;
}

int DynamicScheduler::scheduleResNeed(Device deviceType, Schedule& schedule) 
{
	return -1;
	}