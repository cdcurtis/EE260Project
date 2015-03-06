/*
 * LeveledDag.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: Chris
 */
#include <iostream>
#include <cstdio>
#include <climits>
#include  "../Headers/LeveledDag.h"

using namespace std;
LeveledDag:: ~LeveledDag()
{

	//std::cout << "Calling LeveledDag Destructor"<<std::endl;
	for(unsigned int i =0 ; i< levels.size();++i)
		for(unsigned int j=0; j< levels[i].size();++j)
		{
			//			std:: cout << i << " " << j <<std::endl;
			delete levels[i][j];

		}

	//	std::cout << "Leaving LeveledDag Destructor"<<std::endl;
}

LeveledDag:: LeveledDag(DagGen *dag, std::string nme,int timeStarted, int deadline, int dagNum, bool renameNode):criticalPathSize(-1), priority(NotSpecified)
{
	this->name = nme;
	this->startingAt = timeStarted;
	this->Deadline = deadline;
	ID = 0;
	std:: map<int, ScheduleNode*> ::iterator child;
	std:: map<int, ScheduleNode*> ::iterator parent;

	std:: map<int, ScheduleNode*> ScheduleNodes;
	for(unsigned int i=0; i<dag->vertices.size(); ++i){
		char buffer[50];
		sprintf(buffer,"%s", string(dag->vertices[i]->label).c_str());
		if(renameNode) {
			sprintf(buffer,"D:%iN:%i", dagNum, dag->vertices[i]->uniqueID);
			Vertex* OP = dag->vertices[i];
			switch (OP->type) {
			case DISPENSE:
				sprintf(buffer,"D:%iN:DIS:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case DETECT:
				sprintf(buffer,"D:%iN:DET:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case MIX:
				sprintf(buffer,"D:%iN:MIX:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case HEAT:
				sprintf(buffer,"D:%iN:HEAT:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case SPLIT:
				sprintf(buffer,"D:%iN:SPT:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case STORE:
				sprintf(buffer,"D:%iN:STR:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case WASTE:
				sprintf(buffer,"D:%iN:WST:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			case OUTPUT:
				sprintf(buffer,"D:%iN:OUT:%i",dagNum, dag->vertices[i]->uniqueID);
				break;
			default:
				break;
			}
		}
		ScheduleNodes.insert(std::pair<int, ScheduleNode*>(dag->vertices[i]->uniqueID, new ScheduleNode(dag->vertices[i],buffer)));
	}
	for(unsigned int i=0; i< dag->edges.size(); ++i)
	{
		child = ScheduleNodes.find(dag->edges[i]->child);
		parent = ScheduleNodes.find(dag->edges[i]->parent);

		child->second->parents.push_back(parent->second);
		parent->second->children.push_back(child->second);
	}

	while(! ScheduleNodes.empty())
	{
		std::vector<ScheduleNode*> level;
		for( std:: map<int, ScheduleNode*> ::iterator it = ScheduleNodes.begin(); it != ScheduleNodes.end(); it++ )
		{
			bool parentFound = false;
			std:: map<int, ScheduleNode*> ::iterator parentFinder;
			for(unsigned int i=0; i< it->second->parents.size(); ++i)
			{
				parentFinder = ScheduleNodes.find(it->second->parents[i]->uniqueID);
				if(parentFinder != ScheduleNodes.end())
				{
					parentFound = true;
					break;
				}
			}
			if(! parentFound)
				level.push_back(it->second);
		}
		for(unsigned int i = 0; i< level.size();++i)
		{
			std:: map<int, ScheduleNode*> ::iterator it = ScheduleNodes.find(level[i]->uniqueID);

			if(it != ScheduleNodes.end())
				ScheduleNodes.erase(it);
		}
		levels.push_back(level);
	}
	Rebalance();
}

void LeveledDag:: DeleteNode(ScheduleNode* node)
{
	int level = FindLevel(node);

	for(vector<ScheduleNode*>::iterator finder = levels[level].begin(); finder != levels[level].end(); ++finder)
	{
		if(*finder == node) {
			levels[level].erase(finder);
			return;
		}
	}
}
void LeveledDag:: Rebalance()
{
	vector<ScheduleNode*> reevaluateList;
	for(unsigned int level =0 ; level < levels.size() ; ++level) {
		for(vector<ScheduleNode*>::iterator node = levels[level].begin(); node != levels[level].end(); ) {
			int minLevel = levels.size();
			for(vector<ScheduleNode*>::iterator childNode = (*node)->children.begin() ; childNode !=(*node)->children.end(); ++childNode) {
				if((*childNode)->type == WASTE || (*childNode)->type == OUTPUT) {
					reevaluateList.push_back(*childNode);
					continue;
				}
				int childLevel = FindLevel(*childNode);
				if(childLevel < minLevel)
					minLevel = childLevel;
			}
			if((*node)->type == WASTE || (*node)->type == OUTPUT) {
				reevaluateList.push_back(*node);
				++node;
				continue;
			}
			else if(minLevel - 1 != level && minLevel != levels.size()) {

				reevaluateList.push_back(*node);
				levels[minLevel-1].push_back(*node);
				levels[level].erase(node);
			}
			else
			{
				++node;
			}
		}
	}
	while(reevaluateList.size() !=0 )
	{
		vector<ScheduleNode*> ::iterator node = reevaluateList.begin();
		int currentLevel = FindLevel(*node);
		if((*node)->type ==WASTE || (*node)->type==OUTPUT){//take the next level after parent
			int parentLevel = FindLevel((*node)->parents[0]);
			if(parentLevel+1 != currentLevel){
				DeleteNode(*node);
				levels[parentLevel+1].push_back(*node);
			}
		}
		else {

			for(vector<ScheduleNode*> ::iterator parentNode = (*node)->parents.begin(); parentNode!= (*node)->parents.end(); ++parentNode ) {
				int parentLevel = FindLevel(*parentNode);
				int minLevel = levels.size();
				for(vector<ScheduleNode*> ::iterator parentsChildNode = (*parentNode)->children.begin(); parentsChildNode!= (*parentNode)->children.end(); ++parentsChildNode ) {
					int childLevel = FindLevel(*parentsChildNode);

					if (minLevel > childLevel)
						minLevel = childLevel;
				}
				if(minLevel - 1 != parentLevel) {
					DeleteNode(*parentNode);
					reevaluateList.push_back(*parentNode);
					levels[minLevel-1].push_back(*parentNode);
				}
			}


		}
		reevaluateList.erase(node);
	}
}

int LeveledDag:: FindLevel(ScheduleNode* node)
{
	for(unsigned int level=0; level <levels.size(); ++level)
	{
		for( unsigned int nodeIndex = 0; nodeIndex <levels[level].size(); ++nodeIndex)
		{
			if(*levels[level][nodeIndex] == *node)
				return level;
		}
	}

	return -1;
}

bool LeveledDag:: CanNodeBeDelayed (ScheduleNode* node, int curentLevel)
{
	int myLevel = this->FindLevel(node);
	if(myLevel == -1) {
		cerr<<"Node "<< node->label << " is Not Found\n";
		return false;
	}

	if (myLevel< curentLevel)
		myLevel =curentLevel;

	if(node->children.size()==0)
		return false;
	int minChildLevel = INT_MAX;
	for (unsigned int i = 0; i< node->children.size(); ++i) {

		int childLevel = this->FindLevel(node->children[i]);
		if (childLevel == myLevel + 1)
			return false;
		else if(minChildLevel > childLevel)
			minChildLevel = childLevel;

	}

	return minChildLevel-1 > myLevel;
}
void LeveledDag:: print()
{
	for(unsigned int i=0; i< levels.size(); ++i)
	{
		std:: cout << "Level: " << i<< " :  " ;
		for (unsigned int j=0; j < levels[i].size(); ++j)
			std:: cout<< levels[i][j]->label << " ";
		std:: cout << std::endl;
	}

}

