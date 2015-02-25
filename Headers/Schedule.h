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
	/*
		Function: CreateNewTimeStep
		Parameters:
		
		Adds a new vector to the bottom the of availableModulesAtTimeStep.
	*/
	void CreateNewTimeStep();
	
	/*
		Function: CanAddOperationStartingAtTime
		Parameters: Schedule Node to be inserted, Time step to start looking. 
		Returns: True if the scheduleNode can be insert. otherwise false.
		
		Checks the device constraints on the operation also the time needed within the Vertex, 
		and we take the max between the two to decide the running time(N) of the operation. 
		We check the next N time steps to see if the node can be added here. 
	*/
	bool CanAddOperationStartingAtTime(ScheduleNode*, int);

	/*
		Function: AddOperationStartingAtTime
		Parameters: Schedule Node to be inserted, Time step to start looking. 
		Returns: The ending time position of the operation, or -1 for fail.
		
		This function checks to see if the node can be inserted, if it cannot then returns -1. 
		Otherwise it adds the ScheduledNode to the schedule and returns the endtime of the 
		operation.
	*/
	int AddOperationStartingAtTime(ScheduleNode*, int);

	/*
		Function: RemoveOperationAt
		Parameter: TimeStep, NodeIndex
		Return: true if successfully removes operation, false if an error occurred. 
		
	*/
	bool RemoveOperatationAt(unsigned int,unsigned int);
	
	
private:
	
	void ReplaceModule(ScheduleNode*, int);
	bool CanAddOperationAtTime(ScheduleNode*, int, int& );
	bool CanAddOperationAtTime(ScheduleNode*, int);
	bool AddOperationAtTime(ScheduleNode*, int );
	double GetOperationTime(VertexType);
	Device* _device;
	OperationTimer _timer;
};

#endif // __SCHEDULE_H__
