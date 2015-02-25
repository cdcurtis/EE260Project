/*
 * LeveledDag.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: Chris
 */
#include <iostream>
#include  "../Headers/LeveledDag.h"

LeveledDag:: ~LeveledDag()
{
#ifdef DEBUG_DESTRUCTOR
	std::cout << "Calling Destructor"<<std::endl;
#endif
	for(unsigned int i =0 ; i< levels.size();++i)
		for(unsigned int j=0; j< levels[i].size();++j)
		{
			//std:: cout << i << " " << j <<std::endl;
			delete levels[i][j];

		}
#ifdef DEBUG_DESTRUCTOR
	std::cout << "Leaving Destructor"<<std::endl;
#endif
}
LeveledDag:: LeveledDag(DagGen dag): DagGen(dag),criticalPathSize(-1), priority(NotSpecified)
{

	std:: map<int, ScheduleNode*> ::iterator child;
	std:: map<int, ScheduleNode*> ::iterator parent;

	std:: map<int, ScheduleNode*> ScheduleNodes;
	for(unsigned int i=0; i<vertices.size(); ++i){

		ScheduleNodes.insert(std::pair<int, ScheduleNode*>(vertices[i]->uniqueID, new ScheduleNode(*vertices[i])));
	}
	for(unsigned int i=0; i< edges.size(); ++i)
	{
		child = ScheduleNodes.find(edges[i]->child);
		parent = ScheduleNodes.find(edges[i]->parent);

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

