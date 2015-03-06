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
		
	  /* ===================================================================================
	  Default Constructor
	
	  @params
	  @returns
	  ====================================================================================*/
		DynamicScheduler();


		/* ===================================================================================
  	Constructer with vector of DagGens passed in 
	
	  @params
	  @returns
	  =================================================================================== */
		DynamicScheduler(std:: vector<DagGen>);


	  /* ===================================================================================
	  Destructor
	
	  @params
	  @returns
	  =================================================================================== */

		~DynamicScheduler();
	
		// Class Methods
		/*
		JP: 	 FIFO, Resources Needed
		Chris: Operations ready, Critical Path
		*/
		
		int schedule(ScheduleType, Schedule*, std::vector<LeveledDag*>&);
		int scheduleFIFO(Schedule*, std::vector<LeveledDag*>&);
		int scheduleResNeed(Schedule*, std::vector<LeveledDag*>&);
		int scheduleOpReady(Schedule*, std::vector<LeveledDag*>&);
		int scheduleCritPath(Schedule*, std::vector<LeveledDag*>&);
		void ScheduleDag(Schedule* , LeveledDag*, int =-1);
	private:

		void RipUpNode(Schedule*,ScheduleNode*);
		void RipUpDagAndReScheduleAt(Schedule* , LeveledDag*, int);
		void CalculateCritcalPaths(Schedule*, std::vector<LeveledDag*>&);

};

#endif //__DYNAMICSCHEDULER_H__
