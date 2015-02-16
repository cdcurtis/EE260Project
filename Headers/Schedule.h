#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include "Device.h"
#include "LeveledDag.h"

struct Schedule
{
public:
	std:: vector< std::vector <Module> > availableModulesAtTimestep;
	std:: vector< std::vector <ScheduleNode*> > schduledNodes;

	//takes a ScheduleNode and return the 2D index where it appears in schduledNodes
	std::map <ScheduleNode*, std::pair<int,int> > nodeIndexLookup;


	Schedule(Device* d)
	{
		if(d == NULL){
			std::cerr<<"The device for the schedule cannot be found.\n";
			std::exit(-1);
		}

		//check that the device pointer is valid.
		DMFB* dmfb = dynamic_cast<DMFB*>(d);
		if(dmfb == 0) { //this checks for the null pointers
			std::cerr<<"The device type is not recognized\n";
			std::exit(-2);
		}

		_device = d;
		_timer = d->timer;
	}

	void CreateNewTimeStep();
	bool CanAddOperationStartingAtTime(ScheduleNode*, int);
	bool AddOperationStartingAtTime(ScheduleNode*, int);

	bool RemoveOperatationAt(unsigned int,unsigned int);
	void ReplaceModule(ScheduleNode*, int);

private:
	bool CanAddOperationAtTime(ScheduleNode*, int, int& );
	bool CanAddOperationAtTime(ScheduleNode*, int);
	bool AddOperationAtTime(ScheduleNode*, int );
	double GetOperationTime(VertexType);
	Device* _device;
	OperationTimer _timer;
};

#endif // __SCHEDULE_H__
