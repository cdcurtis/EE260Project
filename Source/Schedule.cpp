/*
 * Schedule.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: Chris
 */
#include <cmath>
#include <iostream>
#include "../Headers/Schedule.h"

using namespace std;

void Schedule:: CreateNewTimeStep()
{
	vector<Module> temp = _device.modules;
	availableModulesAtTimestep.push_back(temp);

	vector<ScheduleNode*> nodes;

	schduledNodes.push_back(nodes);
}

double Schedule:: GetOperationTime(VertexType OP)
{
	string operation;
	switch (OP) {
	case DISPENSE:
		operation = "dispense";
		break;
	case DETECT:
		operation = "detect";
		break;
	case HEAT:
		operation = "heat";
		break;
	case MIX:
		operation = "mix";
		break;
	case SPLIT:
		operation = "split";
		break;
	case STORE:
		operation = "store";
		break;
	case OUTPUT:
		operation = "output";
		break;
	default:
		break;
	}

	map<string, double>::iterator optime;
	optime = _device.timer.MinTimeForOpComp.find(operation);
	if(optime!= _device.timer.MinTimeForOpComp.end())
		return optime->second;
	return 1;
}

int Schedule:: FindFirstOpening(ScheduleNode* Op, int startTime)
{
	for(;startTime < availableModulesAtTimestep.size(); ++startTime) {
		if(CanAddOperationStartingAtTime(Op,startTime))
			return startTime;
	}
	return -1;
}

void Schedule:: ScheduleNodeASAP(ScheduleNode* node)
{
	int startingPoint = this->FindFirstOpening(node);
	int endingPoint = AddOperationStartingAtTime(node, startingPoint);
	if (endingPoint == -1) {
		cerr<<"Finding Starting Point is Not correct.\n";
	}
	else
	{
		node->timeStarted = startingPoint;
		node->timeEnded = endingPoint;
	}
}
void Schedule::Print()
{
	for(unsigned int i=0; i< schduledNodes.size(); ++i) {
		cout<< "Time: "<< i << " ";
		for(unsigned int j =0; j < schduledNodes[i].size(); ++j) {
			cout << schduledNodes[i][j]->label << " ";
		}
		cout<< endl;
	}
}

bool Schedule::CanAddOperationStartingAtTime(ScheduleNode* OP, int startingTime)
{
	double OPTime = max(GetOperationTime(OP->type), OP->timeNeeded);

	int endTime =startingTime+OPTime;
	if (endTime >= availableModulesAtTimestep.size())
	{
		endTime = availableModulesAtTimestep.size()-1;
	}

	for(int i = startingTime; i< endTime;++i)
	{
		if(!CanAddOperationAtTime(OP,i))
			return false;
	}
	return true;
}
int Schedule::AddOperationStartingAtTime(ScheduleNode* OP, int startingTime)
{

	if(!CanAddOperationStartingAtTime(OP,startingTime))
		return -1;

	int OPTime = max(GetOperationTime(OP->type), OP->timeNeeded);


	int endTime =startingTime+OPTime;


	if (endTime >= availableModulesAtTimestep.size())
	{
		for( int i=availableModulesAtTimestep.size();  i<=endTime; ++i)
			this->CreateNewTimeStep();
	}

	for(int i = startingTime; i<= endTime;++i)
		AddOperationAtTime(OP,i);
	return endTime;
}

bool Schedule::CanAddOperationAtTime(ScheduleNode* OP, int time, int& index)
{
	if (time >= availableModulesAtTimestep.size())
		return true;

	//check that my parents are not also here.
	map <ScheduleNode*, std::pair<int,int> >::iterator parentCheck;
	for(unsigned int i=0; i< OP->parents.size(); ++i) {
		parentCheck = nodeIndexLookup.find(OP->parents[i]);
		if(parentCheck!= nodeIndexLookup.end()) {
			if (parentCheck->second.first == time)
				return false;
		}
	}
	//check that my Children are not here.
	map <ScheduleNode*, std::pair<int,int> >::iterator childCheck;
	for(unsigned int i=0; i< OP->children.size(); ++i) {
		childCheck = nodeIndexLookup.find(OP->children[i]);
		if(childCheck!= nodeIndexLookup.end()) {
			if (childCheck->second.first == time)
				return false;
		}
	}

	for(unsigned int i=0; i< availableModulesAtTimestep[time].size(); ++i)
	{ //simple method, check to see if that resource is available.
		switch (OP->type) {
		case DISPENSE:
			if(availableModulesAtTimestep[time][i].CanDispense()) {
				index = i;
				return true;
			}
			break;
		case DETECT:
			if(availableModulesAtTimestep[time][i].CanDetect()) {
				index = i;
				return true;
			}
			break;
		case HEAT:
			if(availableModulesAtTimestep[time][i].CanHeat()) {
				index = i;
				return true;
			}
			break;
		case MIX:
			if(availableModulesAtTimestep[time][i].CanMix()) {
				index = i;
				return true;
			}
			break;
		case SPLIT:
			if(availableModulesAtTimestep[time][i].CanSplit())
				return true;
			break;
		case STORE:
			if(availableModulesAtTimestep[time][i].CanStore()) {
				index = i;
				return true;
			}
			break;
		case OUTPUT:
			if(availableModulesAtTimestep[time][i].CanOutput()) {
				index = i;
				return true;

			}
			break;
		default:
			break;
		}
	}

	return false;
}

bool Schedule::CanAddOperationAtTime(ScheduleNode* OP, int time)
{
	int trash;
	return CanAddOperationAtTime(OP,time,trash);
}

bool Schedule::AddOperationAtTime(ScheduleNode* OP, int time)
{
	if(time >= availableModulesAtTimestep.size())
		this->CreateNewTimeStep();

	int index;
	if(CanAddOperationAtTime(OP,time,index))
	{
		availableModulesAtTimestep.erase(availableModulesAtTimestep.begin()+index);

		int insertIndex = schduledNodes.size();
		schduledNodes[time].push_back(OP);

		nodeIndexLookup.insert(pair<ScheduleNode*,pair<int,int> >(OP,pair<int,int>(time,insertIndex)));
		return false;
	}
	cerr<<"Cannot add Operation "<< OP->label <<" at time: "<< time<< endl;
	return false;
}

bool Schedule::RemoveOperatationAt(unsigned int time,unsigned int nodeIndex)
{
	if(schduledNodes.size()<time){
		cerr<<"Trying to access out of bounds time step\n"<< "Time step:" <<time <<" Size:" << schduledNodes.size()<<endl;
		return false;
	}

	if(schduledNodes[time].size()<nodeIndex){
		cerr<<"Trying to access out of bounds Node\n"<< "Node index:" <<nodeIndex <<" Size:" << schduledNodes[time].size()<<endl;
		return false;
	}

	ScheduleNode* RemoveME = schduledNodes[time][nodeIndex];

	schduledNodes[time].erase(schduledNodes[time].begin()+nodeIndex);

	if(nodeIndexLookup.find(RemoveME) != nodeIndexLookup.end())
		nodeIndexLookup.erase(nodeIndexLookup.find(RemoveME));

	ReplaceModule(RemoveME,time);

	return true;
}

void Schedule::ReplaceModule(ScheduleNode* node,int time)
{

	switch (node->type) {
	case DISPENSE:
	case OUTPUT:
		availableModulesAtTimestep[time].push_back(Module("Dispense/output",0x60,0));
		break;
	case DETECT:
	case HEAT:
	case MIX:
	case SPLIT:
	case STORE:
		availableModulesAtTimestep[time].push_back(Module("WorkerModule",0x1F,2));
		break;
	default:
		break;
	}

}







