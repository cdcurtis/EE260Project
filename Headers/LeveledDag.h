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
	int timeStarted;
	int timeEnded;
	ScheduleNode(Vertex V): Vertex(V), parents(),children(), timeNeeded(0) {	}
	~ScheduleNode()
	{
	#ifdef DEBUG_DESTRUCTOR
		std::cout << "Calling ScheduleNode Destructor"<<std::endl;
		std::cout << "Leaving ScheduleNode Destructor"<<std::endl;
	#endif
	}
	
	
	/*
		Overloaded Comparison operator
			this function is the overloaded equality operator. This allows ScheduleNodes to be compared to each other.
	*/
	
	inline bool operator==( const ScheduleNode& rhs)
	{
		if(this->timeNeeded != rhs.timeNeeded)
			return false;
		if(this->type != rhs.type)
			return false;
		if(this->label != rhs.label)
			return false;
			
		return true;
	}
};

class LeveledDag
{
private:
	double criticalPathSize;
	SchedulePriorty priority;
	std::vector < std:: vector <ScheduleNode*> > levels;

	void CompressLevels();
public:

	~LeveledDag();
	LeveledDag();
	LeveledDag(DagGen dag);
	
	/*
	Function: Print
	Parameters: 
	
	Currently a simple command line output to allow debugging. Outputs each level by line. 
	*/
	void print();
	int FindLevel(ScheduleNode*);
	bool CanNodeBeDelayed (ScheduleNode*, int = -1);
	double & CriticalPathSize() { return criticalPathSize; }
	std::vector < std:: vector <ScheduleNode*> > Levels() { return levels; }

};

#endif //__LEVELEDDAG_H__
