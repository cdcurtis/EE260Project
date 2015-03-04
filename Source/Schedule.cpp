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
	for(unsigned int parent=0; parent < Op->parents.size(); ++parent){

		if(startTime <= Op->parents[parent]->timeEnded)
			startTime =  Op->parents[parent]->timeEnded;
	}


	for(;startTime <= availableModulesAtTimestep.size(); ++startTime) {
		if(this->CanAddOperationStartingAtTime(Op,startTime))
			return startTime;
	}
	return startTime;
}
void Schedule::CreateStore(ScheduleNode* parent, ScheduleNode* child, int endtime ){
	string storeLabel = "S:" + parent->label;
	cout << storeLabel<<endl;
	Vertex *v = new Vertex(STORE,storeLabel,endtime);
	ScheduleNode * newStore = new ScheduleNode(v);
	delete v;
	newStore->parents.push_back(parent);
	newStore->children.push_back(child);

	for(vector<ScheduleNode*>::iterator parentIterator = child->parents.begin(); parentIterator != child->parents.end(); ++parentIterator) {
		if (parent == (*parentIterator)){
			child->parents.erase(parentIterator);
			break;
		}
	}
	child->parents.push_back(newStore);

	for(vector<ScheduleNode*>::iterator childIterator = parent->children.begin(); childIterator != parent->children.end(); ++childIterator) {
		if (child == (*childIterator)){
			parent->children.erase(childIterator);
			break;
		}
	}

	parent ->children.push_back(newStore);

	for(int startingPoint= parent->timeEnded+1; startingPoint <= endtime; ++startingPoint)
	{
		this->AddOperationAtTime(newStore,startingPoint);
	}
}

void Schedule:: PutNodeInSchdeule(ScheduleNode* node){
	cout<< node->label<<endl;
	for(unsigned int child=0; child < node->children.size(); ++child){
		if (node->children[child]->parents.size() > 1)
			return ScheduleNodeToBalanceChildParents(node);
	}

	return ScheduleNodeASAP(node);

}
void Schedule::ScheduleNodeToBalanceChildParents(ScheduleNode* node){
	int siblingMinEndtime= -1;

	for(unsigned int child=0; child < node->children.size(); child++) {
		for(unsigned int parent = 0; parent< node->children[child]->parents.size(); ++parent) {
			ScheduleNode* sibling = node->children[child]->parents[parent];
			if(sibling == node)
				continue;
			int siblingEndtime = EstimatedEndTime(sibling);
			if(siblingEndtime < siblingMinEndtime || siblingMinEndtime == -1)
				siblingMinEndtime = siblingEndtime;
		}
	}
	int earlyStarting = this->FindFirstOpening(node);
	int myStartingPoint = siblingMinEndtime - this->GetOperationTime(node->type);

	if(earlyStarting > myStartingPoint)
		myStartingPoint = earlyStarting;
	if (myStartingPoint < 0)
		myStartingPoint = 0;
	int endTime=-1;
	do {
		node->timeStarted = myStartingPoint;
		endTime = AddOperationStartingAtTime(node, myStartingPoint++);

	}while (endTime == -1);
	node->timeEnded = endTime;
}

int Schedule:: EstimatedEndTime(ScheduleNode* node)
{
	int maxEnd =-1;
	for(unsigned int parent=0; parent < node->parents.size(); ++parent)
	{
		int parentEndTime= node->parents[parent]->timeEnded;
		if(parentEndTime ==-1)
			parentEndTime = EstimatedEndTime(node->parents[parent]);
		if (parentEndTime > maxEnd)
			maxEnd= parentEndTime;
	}
	if(maxEnd == -1)
		return this->GetOperationTime(node->type);
	return maxEnd + this->GetOperationTime(node->type);
}

void Schedule:: ScheduleNodeASAP(ScheduleNode* node)
{
	int startingPoint = this->FindFirstOpening(node);
	int endingPoint = this->AddOperationStartingAtTime(node, startingPoint);

	if (startingPoint == -1) {
		cerr<<"Finding Starting Point is Not correct.\n";
	}
	else
	{
		node->timeStarted = startingPoint;
		node->timeEnded = endingPoint;
	}

	//add store nodes as necessary.
	for(unsigned int i=0; i< node->children.size(); ++i) {
		unsigned int numParents = node->children[i]->parents.size();
		if (numParents > 1)
		{
			for(unsigned int pIndex=0; pIndex< numParents; ++pIndex) {
				if (node->children[i]->parents[pIndex]->timeEnded == -1);//do nothign
				else if(node->children[i]->parents[pIndex]->timeEnded < node->timeEnded){
					cout<<node->children[i]->parents[pIndex]->timeEnded << " " << node-> timeEnded <<endl;
					CreateStore(node->children[i]->parents[pIndex], node->children[i], node->timeEnded);
				}
				else if (node->children[i]->parents[pIndex]->timeEnded > node->timeEnded){
					CreateStore(node, node->children[i], node->children[i]->parents[pIndex]->timeEnded);
				}
			}
		}
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
	if(startingTime < 0)
		return -1;
	if(!this->CanAddOperationStartingAtTime(OP,startingTime))
		return -1;

	int OPTime = max(GetOperationTime(OP->type), OP->timeNeeded);


	int endTime =startingTime+OPTime;

	for(int i = startingTime; i<endTime;++i)
		this->AddOperationAtTime(OP,i);
	return endTime;
}

bool Schedule::CanAddOperationAtTime(ScheduleNode* OP, int time, int& index)
{
	if (time >= availableModulesAtTimestep.size())
		return true;

	//check that my parents are not also here.
	for(unsigned int i=0; i< OP->parents.size(); ++i) {
		if(time< OP->parents[i]->timeEnded)
			return false;
	}
	//check that my Children are not here.

	for(unsigned int i=0; i< OP->children.size(); ++i) {
		if(OP ->children[i]->timeStarted >=0)
			if (time >= OP->children[i]->timeStarted)
				return false;
	}
	//check for module that is already used as store.
	if(OP->type == STORE) {
		for(unsigned int i=0; i< availableModulesAtTimestep[time].size(); ++i) {
			if(availableModulesAtTimestep[time][i].NumStorageUsed() > 0){
				index = i;
				return true;
			}
		}
	}

	for(unsigned int i=0; i< availableModulesAtTimestep[time].size(); ++i)
	{ //simple method, check to see if that resource is available.
		bool isUsedAsStorage = availableModulesAtTimestep[time][i].NumStorageUsed() > 0;
		switch (OP->type) {
		case DISPENSE:
			if(availableModulesAtTimestep[time][i].CanDispense() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case DETECT:
			if(availableModulesAtTimestep[time][i].CanDetect() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case HEAT:
			if(availableModulesAtTimestep[time][i].CanHeat() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case MIX:
			if(availableModulesAtTimestep[time][i].CanMix()&& !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case SPLIT:
			if(availableModulesAtTimestep[time][i].CanSplit()&& !isUsedAsStorage)
				return true;
			break;
		case STORE:
			if(availableModulesAtTimestep[time][i].CanStore()) {
				index = i;
				return true;
			}
			break;
		case WASTE:
		case OUTPUT:
			if(availableModulesAtTimestep[time][i].CanOutput()&& !isUsedAsStorage) {
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

	while(time >= this->availableModulesAtTimestep.size())
		this->CreateNewTimeStep();
	int index;
	if(CanAddOperationAtTime(OP,time,index))
	{
		if(OP->type==STORE)
		{
			if(availableModulesAtTimestep[time][index].StoreNode()){
				if (availableModulesAtTimestep[time][index].IsStorageFull())
					availableModulesAtTimestep[time].erase(availableModulesAtTimestep[time].begin()+index);
			}
			else
				cerr<<"Error in trying to add a storage node to a full module.\n";
		}
		else
		{
			availableModulesAtTimestep[time].erase(availableModulesAtTimestep[time].begin()+index);
		}

		int insertIndex = schduledNodes.size();
		schduledNodes[time].push_back(OP);

		nodeIndexLookup.insert(pair<ScheduleNode*,pair<int,int> >(OP,pair<int,int>(time,insertIndex)));
		return true;
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







