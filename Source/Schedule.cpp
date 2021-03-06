/*
 * Schedule.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: Chris
 */
#include <cmath>
#include <fstream>
#include <iostream>
#include "../Headers/Schedule.h"

using namespace std;

void Schedule:: CreateNewTimeStep()
{
	//this is a test for the hard coding value.
	vector<Module*> temp;
	for(unsigned int m= 0; m <_device->modules.size(); ++m){
		Module* newMod = new Module();
		newMod->moduleName =_device->modules[m]->moduleName;
		newMod->enabledOperations=_device->modules[m]->enabledOperations;
		newMod->storageCapacity = _device->modules[m]->storageCapacity;
		newMod->usedStorage = _device->modules[m]->usedStorage;
		temp.push_back(newMod);
	}
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
	optime = _device->timer.MinTimeForOpComp.find(operation);
	if(optime!= _device->timer.MinTimeForOpComp.end())
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
bool Schedule::CreateStore(ScheduleNode* parent, ScheduleNode* child, int endtime ){
	string storeLabel = "S:" + parent->label;
	//cout << storeLabel<<endl;
	Vertex *v = new Vertex(STORE,storeLabel,endtime);
	ScheduleNode * newStore = new ScheduleNode(v);
	delete v;
	v= NULL;
	newStore->parents.push_back(parent);
	newStore->children.push_back(child);

	for(vector<ScheduleNode*>::iterator parentIterator = child->parents.begin(); parentIterator != child->parents.end(); ) {
		if (parent == (*parentIterator)){
			//			cerr<< "Removing Node"<< (*parentIterator)->label<<" from the parent of " << child->label <<endl;
			child->parents.erase(parentIterator);
			break;
		}
		else ++parentIterator;
	}
	child->parents.push_back(newStore);
	//	cerr<< "Adding Node"<< (newStore)->label<<" to the parent of" << child->label <<endl;
	for(vector<ScheduleNode*>::iterator childIterator = parent->children.begin(); childIterator != parent->children.end();) {
		if (child == (*childIterator)){
			//			cerr<< "Removing Node"<< (*childIterator)->label<<" from the children of " << parent->label <<endl;
			parent->children.erase(childIterator);
		}
		else
			++childIterator;
	}
	//	cerr<< "Adding Node"<< (newStore)->label<<" to the children of" << parent->label <<endl;
	parent ->children.push_back(newStore);

	newStore->timeStarted = parent->timeEnded;
	newStore->timeEnded = endtime;
	for(int startingPoint= parent->timeEnded; startingPoint < endtime; ++startingPoint)
	{
		if(! this->AddOperationAtTime(newStore,startingPoint))
			return false;
	}

	return true;
}
bool Schedule:: PutNodeInSchdeule(ScheduleNode* node, LeveledDag* dag, int startingPoint){
	/*for(unsigned int child=0; child < node->children.size(); ++child){
		if (node->children[child]->parents.size() > 1)
			return ScheduleNodeToBalanceChildParents(node);
	}*/
	if(startingPoint==-1)
		startingPoint =0;
	if( ! ScheduleNodeToBalanceChildParents(node, dag, startingPoint)){
		cerr <<"BROKE STORE NODE "<<endl;
		return false;
	}

	for(unsigned int parentsIndex=0 ;parentsIndex < node->parents.size(); ++parentsIndex)
	{
		if(node->parents[parentsIndex]->timeEnded != node ->timeStarted){
			//cerr <<"BROKE NODE "<< node->label << " AT TIME"<< node->timeStarted <<endl << "\tParent"<<node->parents[parentsIndex]->label <<" Ends at: "<< node->parents[parentsIndex]->timeEnded <<endl;
			/*int gap =node->timeStarted - node->parents[parentsIndex]->timeEnded;
			if(gap >0 && gap < this->gapTolerance ){
				if(this->CreateStore(node->parents[parentsIndex],node,node->timeStarted))
					return true;
				else
					return false;
			}
			else*/
			return false;
		}
	}
	return true;

}

bool Schedule::AddStoreIfNecessary(ScheduleNode* node, bool checkParents)
{
	//checks logical siblings
	for(unsigned int i=0; i< node->children.size(); ++i) {
		unsigned int numParents = node->children[i]->parents.size();
		if (numParents > 1)
		{
			for(unsigned int pIndex=0; pIndex< numParents; ++pIndex) {
				if (node->children[i]->parents[pIndex]->timeEnded == -1)
					continue;//do nothign
				else if(node->children[i]->parents[pIndex]->timeEnded < node->timeEnded){
					//cout<<node->children[i]->parents[pIndex]->timeEnded << " " << node-> timeEnded <<endl;
					if(! CreateStore(node->children[i]->parents[pIndex], node->children[i], node->timeEnded))
						return false;
				}
				else if (node->children[i]->parents[pIndex]->timeEnded > node->timeEnded){
					if(! CreateStore(node, node->children[i], node->children[i]->parents[pIndex]->timeEnded))
						return false;
				}
			}
		}
	}

	//checks parents
	//TODO:: this is somehting to look into later.
	if(checkParents && false ){
		for(vector<ScheduleNode*>::iterator parent = node->parents.begin(); parent != node->parents.end(); ++parent )
		{
			if((*parent)->timeEnded != node->timeStarted)
				if(! CreateStore((*parent),node,node->timeStarted))
					return false;
		}
	}
	return true;
}

bool Schedule::ScheduleNodeToBalanceChildParents(ScheduleNode* node, LeveledDag * dag, int startingPoint){
	//cout<<"Inside Balance"<<endl;

	if(startingPoint==-1)
		startingPoint=0;
	int siblingMinEndtime= -1;


	if(node->parents.size() > 0){
		for(unsigned int child=0; child < node->children.size(); child++) {
			for(unsigned int parent = 0; parent< node->children[child]->parents.size(); ++parent) {
				ScheduleNode* sibling = node->children[child]->parents[parent];
				if(sibling == node)
					continue;
				//			cout<<"Inside EstimatedEndTIme"<<endl;
				int siblingEndtime = EstimatedEndTime(sibling,startingPoint);
				//		cout<<"Outside EstimatedEndTIme"<<endl;
				if(siblingEndtime < siblingMinEndtime || siblingMinEndtime == -1)
					siblingMinEndtime = siblingEndtime;
			}
		}
	}
	else{
		int myLevel = dag->FindLevel(node);
		siblingMinEndtime =-1;//we are actually using this as a max here
		if(myLevel == 0){
			ScheduleNodeASAP(node,startingPoint);
			return true;
		}
		for(unsigned int siblingIndex = 0; siblingIndex < dag->Levels().at(myLevel).size(); ++siblingIndex  )
		{
			ScheduleNode* sibling =  dag->Levels().at(myLevel).at(siblingIndex);
			if(!(*sibling == *node))
			{

				int siblingEndtime = EstimatedEndTime(sibling,startingPoint);
				if(siblingEndtime > siblingMinEndtime)
					siblingMinEndtime = siblingEndtime;
			}
		}
	}


	//	cout<<"Inside FirstOpening"<<endl;
	int earlyStarting = this->FindFirstOpening(node,startingPoint);
	//	cout<<"Outside FirstOpening"<<endl;
	int myStartingPoint = siblingMinEndtime - this->GetOperationTime(node->type);

	if(earlyStarting > myStartingPoint)
		myStartingPoint = earlyStarting;
	if (myStartingPoint < 0)
		myStartingPoint = 0;
	int endTime=-1;
	do {
		node->timeStarted = myStartingPoint;
		//	cout<<"Inside AddOppStarting"<<endl;
		endTime = AddOperationStartingAtTime(node, myStartingPoint++);
		//	cout<<"Outside AddOppStarting"<<endl;
	}while (endTime == -1);
	node->timeEnded = endTime;

	//cout<<"Timing: "<< node->timeStarted << "-> " << node->timeEnded <<endl;


	//add StoresIf Necessary
	return AddStoreIfNecessary(node, true);
}

int Schedule:: EstimatedEndTime(ScheduleNode* node, int startingPointOfDag)
{

	if(node->timeEnded != -1)
		return node->timeEnded;
	int maxEnd =-1;
	for(unsigned int parent=0; parent < node->parents.size(); ++parent)
	{
		int parentEndTime= node->parents[parent]->timeEnded;
		if(parentEndTime ==-1)
			parentEndTime = EstimatedEndTime(node->parents[parent],startingPointOfDag);
		if (parentEndTime > maxEnd)
			maxEnd= parentEndTime;
	}
	if(maxEnd == -1)
		return startingPointOfDag + this->GetOperationTime(node->type);
	return maxEnd + this->GetOperationTime(node->type);
}

void Schedule:: ScheduleNodeASAP(ScheduleNode* node, int StartingAt)
{
	if(StartingAt == -1)
		StartingAt=0;
	//cout<<"Inside ASAP"<<endl;
	int startingPoint = this->FindFirstOpening(node,StartingAt);
	int endingPoint = this->AddOperationStartingAtTime(node, startingPoint);

	if (startingPoint == -1) {
		cerr<<"Finding Starting Point is Not correct.\n";
	}
	else
	{
		node->timeStarted = startingPoint;
		node->timeEnded = endingPoint;
		//cout<<"Timing: " << startingPoint << "->" << endingPoint <<endl;
	}

	//add store nodes as necessary.
	this->AddStoreIfNecessary(node, false);
}
void Schedule::Print()
{
	for(unsigned int i=0; i< schduledNodes.size(); ++i) {
		//int opsUsed =0;
		//int stores=0;
		cout<< "Time: "<< i << " ";
		for(unsigned int j =0; j < schduledNodes[i].size(); ++j) {
			try{
				if(schduledNodes[i][j] != NULL){
					/*	if(schduledNodes[i][j]->type ==STORE)
				stores++;
			else
				opsUsed++;
					 */
					cout << schduledNodes[i][j]->label << " ";
				}
			}
			catch(...)
			{
				cerr<<"BadMemory" <<endl;
			}
		}
		cout <<endl;
		//cout<<"\t\t Resoreces Used: "<< opsUsed + (stores/2) << "Amount Left: "<< availableModulesAtTimestep[i].size()<< endl;
	}
}

void Schedule::PrintRes()
{
	int i =0;

	for(vector<vector <ScheduleNode*> >::iterator timeStep = schduledNodes.begin(); timeStep != schduledNodes.end(); ++timeStep, ++i) {
		int opsUsed =0;
		int outUsed =0;
		int stores=0;
		cout<< "Time: "<< i << " ";
		for(vector <ScheduleNode*> ::iterator node = timeStep->begin(); node != timeStep->end(); ++node) {
			//cout << (*node)->label<< " is a ";
			if((*node)->type ==STORE){
				stores++;

			}
			else if ((*node) ->type==DISPENSE || (*node)->type==OUTPUT || (*node)->type==WASTE)
				outUsed++;
			else
				opsUsed++;
		}
		int notused = availableModulesAtTimestep[i].size();
		cout << "\n\tDIS: "<< outUsed << " Ops: " << opsUsed << " Stores: "<<stores<< "\n\tTotal:  "<< notused+outUsed+opsUsed+(stores/2)<< "Available spots:"<< notused<< endl;

	}
}

bool Schedule::isValid()
{

	for(std:: vector< std::vector <ScheduleNode*> >::iterator timeSteps = schduledNodes.begin() ; timeSteps != schduledNodes.end(); ++timeSteps) {
		for(std::vector <ScheduleNode*> ::iterator parent = timeSteps->begin();  parent != timeSteps->end() ; ++parent) {
			for(std::vector <ScheduleNode*> ::iterator child = (*parent)->children.begin() ; child != (*parent)->children.end() ; ++child) {
				if((*child)->timeStarted != (*parent)->timeEnded ){
					cerr<<"Parent: " << (*parent)->label << " does not match with Child: "<< (*child)->label <<endl;
					return false;
				}
			}
		}
	}
	return true;
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

	for(int i = startingTime; i<endTime;++i){
		//cout<<" addingOp at time "<< i<<endl;
		this->AddOperationAtTime(OP,i);
	}
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
			if(availableModulesAtTimestep[time][i]->NumStorageUsed() > 0){
				index = i;
				return true;
			}
		}
	}

	for(unsigned int i=0; i< availableModulesAtTimestep[time].size(); ++i)
	{ //simple method, check to see if that resource is available.
		bool isUsedAsStorage = availableModulesAtTimestep[time][i]->NumStorageUsed() > 0;
		switch (OP->type) {
		case DISPENSE:
			if(availableModulesAtTimestep[time][i]->CanDispense() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case DETECT:
			if(availableModulesAtTimestep[time][i]->CanDetect() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case HEAT:
			if(availableModulesAtTimestep[time][i]->CanHeat() && !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case MIX:
			if(availableModulesAtTimestep[time][i]->CanMix()&& !isUsedAsStorage) {
				index = i;
				return true;
			}
			break;
		case SPLIT:
			if(availableModulesAtTimestep[time][i]->CanSplit()&& !isUsedAsStorage) {
				index=i;
				return true;
			}
			break;
		case STORE:
			if(availableModulesAtTimestep[time][i]->CanStore()) {
				index = i;
				return true;
			}
			break;
		case WASTE:
		case OUTPUT:
			if(availableModulesAtTimestep[time][i]->CanOutput()&& !isUsedAsStorage) {
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
	int index =-1;
	if(CanAddOperationAtTime(OP,time,index))
	{

		if(index==-1)
			return false;
		if(OP->type==STORE)
		{
			if(availableModulesAtTimestep[time][index]->StoreNode()){
				if (availableModulesAtTimestep[time][index]->IsStorageFull()) {
					Module * deleteME = availableModulesAtTimestep[time][index];
					availableModulesAtTimestep[time].erase(availableModulesAtTimestep[time].begin()+index);
					delete deleteME;
					deleteME =NULL;
				}
			}
			else
				cerr<<"Error in trying to add a storage node to a full module.\n";
		}
		else
		{
			Module * deleteME = availableModulesAtTimestep[time][index];
			availableModulesAtTimestep[time].erase(availableModulesAtTimestep[time].begin()+index);
			delete deleteME;
			deleteME= NULL;
		}

		int insertIndex = schduledNodes.size();
		schduledNodes[time].push_back(OP);

		return true;
	}
	cerr<<"Cannot add Operation "<< OP->label <<" at time: "<< time<< endl;
	return false;
}
void Schedule::RemoveScheduleNode(ScheduleNode* Op)
{
	if(Op->timeStarted ==-1)
		return;
	//cout<< "Removing " <<Op->label<<endl;

	int startingTime = Op->timeStarted;
	int endingTime = Op->timeEnded;
	for(int i = startingTime; i < endingTime; ++i)
	{
		int nodeIndex = this->FindScheduleNode(Op,i);
		if(!(RemoveOperatationAt(i,nodeIndex))){
			if(Op->type != STORE)
				cerr<<"Error Removing " << Op->label <<" at " << i<<endl;
		}
	}
	Op->timeEnded=-1;
	Op->timeStarted =-1;
}

bool Schedule::RemoveOperatationAt(unsigned int time,unsigned int nodeIndex)
{
	if(nodeIndex == -1)
		return false;
	/*
	for(unsigned int j =0; j < schduledNodes[time].size(); ++j) {
				cout << schduledNodes[time][j]->label << " ";
			}
	cout<<endl;
	cout <<availableModulesAtTimestep[time].size() << endl;
	 */
	if(schduledNodes.size()<time){
		cerr<<"Trying to access out of bounds time step\n"<< "Time step:" <<time <<" Size:" << schduledNodes.size()<<endl;
		return false;
	}

	if(schduledNodes[time].size()<nodeIndex){
		cerr<<"Trying to access out of bounds Node\n"<< "Node index:" <<nodeIndex <<" Size:" << schduledNodes[time].size()<<endl;
		return false;
	}

	ScheduleNode* RemoveME = schduledNodes[time][nodeIndex];
	//cout <<	(*(schduledNodes[time].begin()+nodeIndex))->label<<endl;

	schduledNodes[time].erase(schduledNodes[time].begin()+nodeIndex);


	ReplaceModule(RemoveME,time);
	/*
	for(unsigned int j =0; j < schduledNodes[time].size(); ++j) {
				cout << schduledNodes[time][j]->label << " ";
			}
	cout<<endl;
	cout <<availableModulesAtTimestep[time].size() << endl;
	 */
	return true;
}

int Schedule::FindScheduleNode(ScheduleNode* Op, int timeStep)
{
	for(unsigned int nodeIndex = 0; nodeIndex < schduledNodes[timeStep].size(); ++ nodeIndex)
	{
		if (Op == schduledNodes[timeStep][nodeIndex])
			return nodeIndex;
	}
	return -1;
}


void Schedule::ReplaceModule(ScheduleNode* node,int time)
{
	bool createNewMod=false;

	switch (node->type) {
	case DISPENSE:
	case WASTE:
	case OUTPUT:
		availableModulesAtTimestep[time].push_back( new Module("Dispense/output",0x60,0));
		break;
	case DETECT:
	case HEAT:
	case MIX:
	case SPLIT:
		availableModulesAtTimestep[time].push_back(new Module("WorkerModule",0x1F,2));
		break;
	case STORE:
		for(unsigned int i = 0; i< availableModulesAtTimestep[time].size(); ++i) {
			if(availableModulesAtTimestep[time][i]->usedStorage > 0){
				availableModulesAtTimestep[time][i]->usedStorage= availableModulesAtTimestep[time][i]->storageCapacity -1;
				return;
			}
		}
		createNewMod = true;
		break;
	default:
		break;
	}


	if(createNewMod){
		Module* newMod = new Module();
		newMod->moduleName = "WorkerNode";
		newMod->enabledOperations = 0x1F;
		newMod->storageCapacity = 2;
		newMod->usedStorage= 1;
		availableModulesAtTimestep[time].push_back(newMod);
	}
}







