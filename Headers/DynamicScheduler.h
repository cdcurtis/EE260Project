#ifndef __DYNAMICSCHEDULER_H__
#define __DYNAMICSCHEDULER_H__

#include <iostream>
#include <vector>
#include "Device.h"
#include "../DagGen/Headers/DagGen.h"

class DynamicScheduler
{
private:

	// Class Builders
	DynamicScheduler();
	DynamicScheduler(std:: vector<DagGen>);
	~DynamicScheduler();
	
	// Class Methods
	
	
	std:: vector< std::vector <Module> > availableModulesAtTimestep;
	//std:: vector< std::vector <ScheduleNode> > schduledNodes
	//std::map <ScheduleNode, std::pair<int,int> > NodeIndexLookup;
};

#endif //__DYNAMICSCHEDULER_H__
