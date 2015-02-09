#ifndef __LEVELEDDAG_H__
#define __LEVELEDDAG_H__


#include <iostream>
#include <map>
#include <vector>
#include "../DagGen/Headers/DagGen.h"

enum SchedulePriorty {High,Medium,Low,NotSpecified};

struct ScheduleNode : public Vertex
{
	std:: vector<ScheduleNode*> parents;
	std::vector<ScheduleNode*> children;
	double timeNeeded;
	ScheduleNode(Vertex V): Vertex(V), parents(),children(), timeNeeded(0) {	}

};

class LeveledDag: public DagGen
{
private:
	SchedulePriorty priority;
	

public:
std::vector < std:: vector <ScheduleNode*> > levels;
	~LeveledDag()
	{
	std::cout << "Calling Destructor"<<std::endl;
		for(int i =0 ; i< levels.size();++i)
			for(int j=0; j<levels[i].size();++j)
				{
					std:: cout << i << " " << j <<std::endl;
					delete levels[i][j];
					
				}
	}

	LeveledDag(DagGen dag): DagGen(dag), priority(NotSpecified)
	{

		std:: map<int, ScheduleNode*> ::iterator child;
		std:: map<int, ScheduleNode*> ::iterator parent;

		std:: map<int, ScheduleNode*> ScheduleNodes;
		for(int i=0; i<vertices.size(); ++i){

			ScheduleNodes.insert(std::pair<int, ScheduleNode*>(vertices[i]->uniqueID, new ScheduleNode(*vertices[i])));
		}
		for(int i=0; i< edges.size(); ++i)
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
				for(int i=0; i< it->second->parents.size(); ++i)
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
			for(int i = 0; i< level.size();++i)
			{
			  std:: map<int, ScheduleNode*> ::iterator it = ScheduleNodes.find(level[i]->uniqueID);
			  
			  if(it != ScheduleNodes.end())
			  	ScheduleNodes.erase(it);		
			}
			levels.push_back(level);
		}
	}

	void print()
	{
		for(int i=0; i< levels.size(); ++i)
		{
			std:: cout << "Level: " << i<< ":"; 
			for (int j=0; j < levels[i].size(); ++j)
				std:: cout<< levels[i][j]->label << " ";
			std:: cout << std::endl;
		}
		
	}

};

#endif //__LEVELEDDAG_H__
