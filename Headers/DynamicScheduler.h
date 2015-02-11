#ifndef __DYNAMICSCHEDULER_H__
#define __DYNAMICSCHEDULER_H__

#include <iostream>
#include <vector>
#include "Device.h"
#include "Schedule.h"
#include "../DagGen/Headers/DagGen.h"

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
		
		int schedule(ScheduleType,Device, Schedule&);
		int scheduleFIFO(Device, Schedule&);
		int scheduleResNeed(Device, Schedule&);
		int scheduleOpReady(Device, Schedule&);
		int scheduleCritPath(Device, Schedule&);

	private:
	

};

#endif //__DYNAMICSCHEDULER_H__
