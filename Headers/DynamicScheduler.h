#ifndef __DYNAMICSCHEDULER_H__
#define __DYNAMICSCHEDULER_H__

#include <iostream>
#include <vector>
#include "Device.h"
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
		
		int schedule(ScheduleType);
		int scheduleFIFO();
		int scheduleResNeed();
		int scheduleOpReady();
		int scheduleCritPath();

	private:

		std:: vector< std::vector <Module> > availableModulesAtTimestep;
		//std:: vector< std::vector <ScheduleNode> > schduledNodes
		//std::map <ScheduleNode, std::pair<int,int> > NodeIndexLookup;
	

};

#endif //__DYNAMICSCHEDULER_H__
