#ifndef __DYNAMICSCHEDULER_H__
#define __DYNAMICSCHEDULER_H__

#include <iostream>
#include <vector>
#include "Device.h"
#include "Schedule.h"
#include "LeveledDag.h"

enum ScheduleType {FIFO,Operations,CriticalPath,ResourcesNeeded};


class DynamicScheduler {

	public:
		// Class Builders
		DynamicScheduler();
		DynamicScheduler(std:: vector<DagGen>);
		~DynamicScheduler();
	
		// Class Methods
		/*
		JP: 	 FIFO, Resources Needed
		Chris: Operations ready, Critical Path
		*/
		
		int schedule(ScheduleType, Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleFIFO(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleResNeed(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleOpReady(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleCritPath(Device*, Schedule&, std::vector<LeveledDag>&);

	private:
	

};

#endif //__DYNAMICSCHEDULER_H__
