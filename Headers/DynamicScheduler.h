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
		
		int schedule(ScheduleType, Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleFIFO(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleResNeed(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleOpReady(Device*, Schedule&, std::vector<LeveledDag>&);
		int scheduleCritPath(Device*, Schedule&, std::vector<LeveledDag>&);

	private:
	

};

#endif //__DYNAMICSCHEDULER_H__
