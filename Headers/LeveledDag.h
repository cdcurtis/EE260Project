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
	~ScheduleNode()
	{
	#ifdef DEBUG_DESTRUCTOR
		std::cout << "Calling ScheduleNode Destructor"<<std::endl;
		std::cout << "Leaving ScheduleNode Destructor"<<std::endl;
	#endif
	}
	
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

class LeveledDag: public DagGen
{
private:
	SchedulePriorty priority;
	std::vector < std:: vector <ScheduleNode*> > levels;

public:

	~LeveledDag();
	LeveledDag(DagGen dag);
	void print();

	std::vector < std:: vector <ScheduleNode*> > Levels() { return levels; }

};

#endif //__LEVELEDDAG_H__
