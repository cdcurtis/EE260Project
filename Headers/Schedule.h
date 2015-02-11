#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <iostream>
#include <vector>
#include <map>
#include "Device.h"
#include "LeveledDag.h"

class Schedule
{
private:
		std:: vector< std::vector <Module> > availableModulesAtTimestep;
		std:: vector< std::vector <ScheduleNode*> > schduledNodes;
		
		//takes a ScheduleNode and return the 2D index where it appears in schduledNodes
		std::map <ScheduleNode, std::pair<int,int> > nodeIndexLookup; 
public: 
		
	Schedule() {}

};

#endif // __SCHEDULE_H__