#ifndef __LEVELEDDAG_H__
#define __LEVELEDDAG_H__


#include <iostream>
#include <map>
#include <vector>
#include "../DagGen/Headers/DagGen.h"

enum SchedulePriorty {High,Medium,Low,NotSpecified};

struct ScheduleNode
{
	VertexType type;
	std :: string label;
	std :: string portName;
	int uniqueID;
	std:: vector<ScheduleNode*> parents;
	std::vector<ScheduleNode*> children;
	double timeNeeded;
	int timeStarted;
	int timeEnded;
	ScheduleNode(Vertex* V, std::string NewLabel= ""): type(V->type), label(V->label),portName(V->portName), uniqueID(V->uniqueID), parents(),children(), timeNeeded(0), timeStarted(-1),timeEnded(-1)
	{
		if(NewLabel!= "")
			this->label = NewLabel;
	}
	~ScheduleNode()
	{

		//std::cout << "Calling ScheduleNode Destructor"<<std::endl;
		//std::cout << "Leaving ScheduleNode Destructor"<<std::endl;
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
	std::string name;
	int startingAt;
	int Deadline;
	int ID;
	double criticalPathSize;
	SchedulePriorty priority;
	std::vector < std:: vector <ScheduleNode*> > levels;

	void CompressLevels();
public:

	~LeveledDag();
	LeveledDag();
	LeveledDag(DagGen* dag,std::string name,int,int,int =-1, bool = false);
	
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
	void Rebalance();
	void DeleteNode(ScheduleNode*);
	std::string& Name() { return name; }
	int& StartingAt() {return startingAt;}
};

#endif //__LEVELEDDAG_H__
