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
int DynamicScheduler::schedule(ScheduleType type) {
	switch(type) {

		case FIFO:
			
		break;
		
		case Operations:
		
		break;
		
		case CriticalPath:
		
		break;
		
		case ResourcesNeeded:
		
		break;
		
		default:
			// FIFO
		break;
	}
	return 0;
}
