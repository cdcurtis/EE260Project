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


	Schedule(DMFB d)
	{
		_device = d;
		_timer = d.timer;
		gapTolerance=5;
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

	void RemoveScheduleNode(ScheduleNode* Op);
	/*
		Function: RemoveOperationAt
		Parameter: TimeStep, NodeIndex
		Return: true if successfully removes operation, false if an error occurred. 
		
	*/
	bool RemoveOperatationAt(unsigned int, unsigned int);

	/*
	 *  Function:  FindScheduleNode
	 *  Parameters:Schedule node to be found
	 *  Return: the node index at the specified timestep. Otherwise -1;
	 */
	int FindScheduleNode(ScheduleNode* Op, int);
	
	/*
	 Function: GetOperationTime
	 Parameters: VertexType(enum)
	 Return: returns min time needed on the device to perform operation.
	*/
	double GetOperationTime(VertexType);

	/*
	 * Function: FindFirstOpening
	 * Parameters: the node to be fitted in, optional starting time point.
	 * Returns the time index of the first open slot in schedule a -1 return if a starting point could not be found. This should never happen.
	 */

	int FindFirstOpening(ScheduleNode*, int startTime = 0);

	bool PutNodeInSchdeule(ScheduleNode*, LeveledDag* , int =-1);
	bool ScheduleNodeToBalanceChildParents(ScheduleNode*, LeveledDag*,int);
	int EstimatedEndTime(ScheduleNode*, int);

	/*
	 * Function SchdeuleNodeASAP
	 * Parameters: ScheduleNode to be added.
	 *
	 * Schedules the Node at the earliest available time.
	 */
	void ScheduleNodeASAP(ScheduleNode*, int);
	/*
	 * Function: Create Store
	 * Parameters Parent, child, Endtime
	 *
	 * creates a dynamic store between parent adn child.
	 * if store cannot schedule Roll Back Dag and Reschedule.
	 */
	bool CreateStore(ScheduleNode*, ScheduleNode*, int);
	void Print();
	bool isValid();
private:
	
	void ReplaceModule(ScheduleNode*, int);
	bool CanAddOperationAtTime(ScheduleNode*, int, int& );
	bool CanAddOperationAtTime(ScheduleNode*, int);
	bool AddOperationAtTime(ScheduleNode*, int );
	DMFB _device;
	OperationTimer _timer;
	int gapTolerance;
};

#endif // __SCHEDULE_H__
