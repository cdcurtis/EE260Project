/*
 * LeveledDag.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: Chris
 */
#include <iostream>
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

LeveledDag:: LeveledDag(DagGen dag):criticalPathSize(-1), priority(NotSpecified)
{

	std:: map<int, ScheduleNode*> ::iterator child;
	std:: map<int, ScheduleNode*> ::iterator parent;

	std:: map<int, ScheduleNode*> ScheduleNodes;
	for(unsigned int i=0; i<dag.vertices.size(); ++i){

		ScheduleNodes.insert(std::pair<int, ScheduleNode*>(dag.vertices[i]->uniqueID, new ScheduleNode(*dag.vertices[i])));
	}
	for(unsigned int i=0; i< dag.edges.size(); ++i)
	{
		child = ScheduleNodes.find(dag.edges[i]->child);
		parent = ScheduleNodes.find(dag.edges[i]->parent);

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
		std:: cout << "Level: " << i<< ":";
		for (unsigned int j=0; j < levels[i].size(); ++j)
			std:: cout<< levels[i][j]->label << " ";
		std:: cout << std::endl;
	}

}

