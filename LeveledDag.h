#ifndef __LEVELEDDAG_H__
#define __LEVELEDDAG_H__


#include <iostream>
#include "DagGen/Headers/DagGen.h"

class LeveledDag: public DagGen
{
	std::vector < std:: vector <ScheduleNode*> > Levels
	
	LeveledDag(DagGen dag){
	
	
	}
	
	~LeveledDag()
	{
		for(int i =0 ; i< levels.size();++i)
			for(int j=0; j<levels[i].size();++j)
				delete levels[i][j];
	}
}

#endif //__LEVELEDDAG_H__