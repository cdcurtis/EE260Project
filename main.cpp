#define DEBUG_DESTRUCTOR

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "Headers/Device.h"
#include "Headers/Schedule.h"
#include "Headers/DynamicScheduler.h"
#include "Headers/LeveledDag.h"
#include "DagGen/Headers/DagGen.h"


using namespace std;

void GenerateFullDAGList(vector<DagGen*>& dags, int number);
void InitalizeDAGs(vector<DagGen*>& dags, int number);

void GenerateGraphic(Schedule*, string = "",bool colorOP = false);
string GetDagName(string N);
int main()
{
	srand(time(NULL));
	vector<DagGen*> startingPoint;
	vector<DagGen*> FullDagList;

	DMFB* board = new DMFB();
	Schedule* CritscheduleStatic = new Schedule(board);
	Schedule* CritscheduleDynamic = new Schedule(board);


	Schedule* FIFOschedule = new Schedule(board);
	DynamicScheduler Scheduler;

	vector<LeveledDag*> CritPathStaticLEVELDAGS;
	vector<LeveledDag*> CritPathDynamicLEVELDAGS;


	vector<LeveledDag*> lg2;


	InitalizeDAGs(startingPoint,256);
	GenerateFullDAGList(FullDagList,256);
	int dagCounter=0;
	for(unsigned int i=0; i < startingPoint.size(); ++i)
	{
		char buffer[20];
		sprintf(buffer,"%i",i);
		//	cout<<i<<endl;
		LeveledDag* LDag= new LeveledDag(startingPoint[i],buffer,i,i,dagCounter,true);
		LeveledDag* LDag2= new LeveledDag(startingPoint[i],buffer,i,i,dagCounter,true);


		CritPathStaticLEVELDAGS.push_back(LDag);
		CritPathDynamicLEVELDAGS.push_back(LDag2);
		dagCounter++;
	}

	//static
	Scheduler.schedule(CriticalPath, CritscheduleStatic,CritPathStaticLEVELDAGS);
	Scheduler.schedule(CriticalPath,CritscheduleDynamic,CritPathDynamicLEVELDAGS);

	GenerateGraphic(CritscheduleDynamic, "CritDagDynamicRd1.html");
	GenerateGraphic(CritscheduleStatic, "CritDagStaticRd1.html");

	vector<LeveledDag*> FullList;
	vector<LeveledDag*> FullListDup;

	for(unsigned int i=0; i < FullDagList.size(); ++i)
	{
		char buffer[20];
		sprintf(buffer,"%i",i);
		LeveledDag* LDag= new LeveledDag(FullDagList[i],buffer,i,i,dagCounter,true);
		LeveledDag* LDagDup= new LeveledDag(FullDagList[i],buffer,i,i,dagCounter,true);

		FullList.push_back(LDag);
		FullListDup.push_back(LDagDup);
		dagCounter++;
	}

	unsigned int timeLimit = CritscheduleDynamic->schduledNodes.size();
	vector<int> dagstoberun;

	unsigned int StaticEndtime;
	//add dag dynamically.
	for(unsigned int timestep = 1; timestep<timeLimit; ++timestep )
	{
		double prob = (double)rand()/(double)RAND_MAX;
		//		cout<<prob <<endl;
		if ( prob >= .85){
			int dagNum = rand() % FullDagList.size();
			cout<< "Adding Dag at time: "<< timestep<<endl;
			Scheduler.ScheduleDag(CritscheduleDynamic,FullList[dagNum],timestep);
			dagstoberun.push_back(dagNum);
		}
	}
	//	secondRoundofStaticDags

	unsigned int StartingPoint = timeLimit;

	for(unsigned int i = 0 ; i < dagstoberun.size(); ++i)
	{
		cout<<dagstoberun[i]<<endl;
		Scheduler.ScheduleDag(CritscheduleStatic,FullListDup[dagstoberun[i]],StartingPoint);
	}
	GenerateGraphic(CritscheduleDynamic, "CritDagDynamicRd2.html");
	GenerateGraphic(CritscheduleStatic, "CritDagStaticRd2.html");


	if(CritscheduleStatic->isValid())
		cout<<"Valid"<<endl;
	else
		cout<<"INVALID"<<endl;
	if(CritscheduleDynamic->isValid())
		cout<<"Valid"<<endl;
	else
		cout<<"INVALID"<<endl;


	cout<<"IT IS FINISHED!"<<endl;
	return 0;
}
void GenerateFullDAGList(vector<DagGen*>& dags, int number)
{
	string PathPrefix = "Inputs\\Remia";
	string PathPosFix = "_256.dag";
	char buffer[50];
	for(int i =1; i< number; ++i) {
		sprintf(buffer,"Inputs\\Remia%i_256.dag",i);
		dags.push_back(new DagGen(buffer));
	}
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

	dags.push_back(new DagGen("Inputs\\Remia1_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia62_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia128_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia8_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia16_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia32_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia13_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia71_256.dag"));
	dags.push_back(new DagGen("Inputs\\Remia205_256.dag"));

	//	}
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
string GetOpName(VertexType type)
{
	switch (type) {

	case DETECT:
	case HEAT:
	case MIX:
	case SPLIT:
		return "SPLIT";
	case STORE:
		return "STORE";
	case OUTPUT:
	case WASTE:
	case DISPENSE:
		return "OUTPUT";

	default:
		return "UNKNOWN";
	}
}
void GenerateGraphic(Schedule* schedule, string fileName,bool colorOP)
{
	map<string,string> dagColors;

	string colors[] = {"0099FF","9933CC","FF9933","FF6699","FFCCFF","CCFF66", "66CC00", "CC6600","B35F5F","60C1D9", "99DD22", "F69C42","F642DB","AC52E9","A2E16A"};
	int maxColors=15;
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
			if(!colorOP)
			{
				if (dagColors.find(dagName) != dagColors.end()) {
					map<string,string>::iterator mycolor = dagColors.find(dagName);
					dagColor = mycolor->second;
				}
				else{
					dagColor = colors[(nextColor++ % maxColors)];
					dagColors.insert(pair<string,string>(dagName, dagColor));

				}
			}
			else{
				string opName = GetOpName (schedule->schduledNodes[timeStep][nodeIndex]->type);
				if (dagColors.find(opName) != dagColors.end()) {
					map<string,string>::iterator mycolor = dagColors.find(opName);
					dagColor = mycolor->second;
				}
				else{
					dagColor = colors[(nextColor++ % maxColors)];
					dagColors.insert(pair<string,string>(opName, dagColor));

				}
			}
			out<<" BGCOLOR=\""<< dagColor << "\">" << label << "</TD>"<<endl;

		}
		out<<"\t\t</TR>"<<endl;
	}



	out<<"\t</TABLE>"<<endl;
	if (&out!=&std::cout)
		delete(&out);
}




