#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <iostream>
#include <vector>
#include <map>

struct OperationTimer
{
	std::string timeUnits;
	std::map<std::string,double> MinTimeForOpComp; //MinTimeForOperationCompletion
	OperationTimer(): timeUnits("Seconds") {}
};

class Module
{
private:


public:
	std:: string moduleName;
	unsigned char enabledOperations;
	int storageCapacity;
	int usedStorage;

	Module(const Module& m ): moduleName(m.moduleName), enabledOperations(m.enabledOperations), storageCapacity(m.enabledOperations), usedStorage(m.usedStorage) {}
	Module() : moduleName(""), enabledOperations(0), storageCapacity(0), usedStorage(0){}
	Module(std:: string n, unsigned char Ops, int storage): moduleName(n), enabledOperations(Ops), storageCapacity(storage), usedStorage(0) {}

	std:: string GetName();
	void SetName( const std::string & name);

	void SetDetect();
	void RemoveDetect();
	bool CanDetect();

	void SetDispense();
	void RemoveDispense();
	bool CanDispense();

	void SetMix();
	void RemoveMix();
	bool CanMix();

	void SetSplit();
	void RemoveSplit();
	bool CanSplit();

	void SetHeat();
	void RemoveHeat();
	bool CanHeat();

	void SetStore(int i);
	void RemoveStore();
	bool CanStore();
	int StorageCapacity();
	int& NumStorageUsed();
	bool IsStorageFull();
	bool StoreNode();

	void SetOutput();
	void RemoveOutput();
	bool CanOutput();	
};


class DMFB
{
public:
	~DMFB()
	{
		for(unsigned int i=0; i<modules.size() ;++i)
			delete modules[i];
	}
	DMFB()
	{
		for(int i=0; i<8 ;++i){
			Module* newMod = new Module();
			newMod ->moduleName ="Worker Node";
			newMod->enabledOperations=0x1f;
			newMod->storageCapacity = 2;
			newMod->usedStorage=0;
			modules.push_back(newMod);
		}
		for(int i=0; i<4 ;++i){
			Module* newMod = new Module();
			newMod ->moduleName ="Dispense/Ouput";
			newMod->enabledOperations=0x60;
			newMod->storageCapacity = 0;
			newMod->usedStorage=0;
			modules.push_back(newMod);
		}

		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("dispense",1));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("detect",5));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("mix",4));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("split",1));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("store",1));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("heat",10));
		timer.MinTimeForOpComp.insert(std::pair<std::string,int>("output",1));

}
	OperationTimer timer;
	std::vector<Module*> modules;
	std::vector<Module*>& Modules() { return modules; }
};

#endif //__DEVICE_H__
