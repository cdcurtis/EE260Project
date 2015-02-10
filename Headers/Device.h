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
	std:: string moduleName;
	unsigned char enabledOperations;
	int numStorage;
	int usedStorage;

public:
	Module() : moduleName(""), enabledOperations(0), numStorage(0), usedStorage(0){}
	Module(std:: string n, unsigned char Ops, int storage): moduleName(n), enabledOperations(Ops), numStorage(storage), usedStorage(0) {}

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
	bool StoreNNodes(int n);

	void SetOutput();
	void RemoveOutput();
	bool CanOutput();	
};

class Device
{
protected:
	OperationTimer timer;
	std::vector<Module> modules;

	Device() {}
	~Device()
	{
#ifdef DEBUG_DESTRUCTOR
		std::cout << "Calling Device Destructor"<<std::endl;
		std::cout << "Leaving Device Destructor"<<std::endl;
#endif
	}
};

class DMFB : public Device
{
public:
	~DMFB()
	{
#ifdef DEBUG_DESTRUCTOR
		std::cout << "Calling DMFB Destructor"<<std::endl;
		std::cout << "Leaving DMFB Destructor"<<std::endl;
#endif
	}
	DMFB()
	{
		for(int i=0; i<8 ;++i)
			modules.push_back(Module("WorkerModule",0x1F,2));

		for(int i=0; i<4 ;++i)
			modules.push_back(Module("Dispense/output",0x60,0));
	}	

	std::vector<Module> Modules() { return modules; }
};

#endif //__DEVICE_H__
