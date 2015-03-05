#define DEBUG_DESTRUCTOR

#include <iostream>
#include <fstream>
#include <cstdio>
#include "Headers/Device.h"
#include "Headers/Schedule.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"
#include "DagGen/Headers/DagGen.h"


using namespace std;


void InitalizeDAGs(vector<DagGen*>& dags, int number);

void GenerateDoty(Schedule*, string = "");
string GetDagName(string N);
int main()
{
	cout << "Starting"<<endl;
	vector<DagGen*> startingPoint;
	InitalizeDAGs(startingPoint,256);
	DMFB board;
	Schedule* schedule = new Schedule(board);
	DynamicScheduler Scheduler;

	vector<LeveledDag*> lg;
	//	startingPoint[0]->generateDotyGraph();
	for(unsigned int i=0; i < startingPoint.size(); ++i)
	{
		//	cout<<i<<endl;
		LeveledDag* LDag= new LeveledDag(startingPoint[i],i,true);
		//	startingPoint[0]->generateDotyGraph("Test.dot");
		//LDag->print();
		lg.push_back(LDag);

	}

	Scheduler.schedule(CriticalPath, schedule,lg);

	schedule->Print();
	GenerateDoty(schedule, "Schedule.html");
	//GenerateDoty(schedule);

	return 0;
}
string GetDagName(string N)
{
	if (N.find("N") ==-1)
	{
		cerr<<"Need to run schedul through rename Labels."<<endl;
		return "error";
	}
	int d = N.find('D');
	int n = N.find('N');
	return N.substr(d,n-d);
}
string GetFriendlyName( string N)
{
	if (N.find("N") ==-1)
	{
		cerr<<"Need to run schedul through rename Labels."<<endl;
		return "error";
	}
	return "Node_" + N.substr(N.find("N")+2);
}
void GenerateDoty(Schedule* schedule, string fileName)
{
	map<string,string> dagColors;

	string colors[] = {"0099FF","9933CC","FF9933","FF6699","FFCCFF","CCFF66", "66CC00", "CC6600"};
	int maxColors=8;
	int nextColor=0;

	if(schedule == NULL)
		return;
	std:: ostream& out = (fileName != "") ? *(new std::ofstream(fileName.c_str())) : std::cout;


	out<<"\t<TABLE ALIGN=\"LEFT\">"<<endl;

	for(unsigned int timeStep = 0; timeStep< schedule->schduledNodes.size(); ++timeStep) {
		out<<"\t\t<TR>"<<endl;
		out <<"\t\t\t<TD>"<<timeStep<< "</TD>"<<endl;

		for(unsigned int nodeIndex =0 ; nodeIndex < schedule->schduledNodes[timeStep].size(); ++nodeIndex ) {
			out<<"\t\t\t<TD";
			string label = schedule->schduledNodes[timeStep][nodeIndex]->label;
			string dagName = GetDagName(label);
			//cout<<dagName<<endl;
			string friendlyName = GetFriendlyName(label);

			string dagColor = "";
			if (dagColors.find(dagName) != dagColors.end()) {
				map<string,string>::iterator mycolor = dagColors.find(dagName);
				dagColor = mycolor->second;
			}
			else{
				dagColor = colors[(nextColor++ % maxColors)];
				dagColors.insert(pair<string,string>(dagName, dagColor));

			}

			out<<" BGCOLOR=\""<< dagColor << "\">" << label << "</TD>"<<endl;

		}
		out<<"\t\t</TR>"<<endl;
	}



	out<<"\t</TABLE>"<<endl;
	if (&out!=&std::cout)
		delete(&out);
}


void InitalizeDAGs(vector<DagGen*>& dags, int number)
{
	string PathPrefix = "Inputs\\Remia";
	string PathPosFix = "_256.dag";
	char buffer[50];
	//int i=62;
	//for(int i =1; i< number; ++i) {
	//	sprintf(buffer,"Inputs\\Remia%i_256.dag",i);
	//dags.push_back(new DagGen(buffer));
	//dags.push_back(new DagGen("Inputs\\Remia1_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia62_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia128_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia8_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia16_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia32_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia13_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia71_256.dag"));
	//dags.push_back(new DagGen("Inputs\\Remia205_256.dag"));

	//	}
}

