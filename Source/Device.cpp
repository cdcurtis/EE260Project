	#include "../Headers/Device.h"
	
///////////////////////////////////////////////////////////////////////////////	
/*     																NAME		  												     */	
///////////////////////////////////////////////////////////////////////////////
	std::string Module:: GetName() { return moduleName; }
	void Module:: SetName( const std:: string& n) { moduleName = n; }
	
///////////////////////////////////////////////////////////////////////////////	
/*     																MIX																     */	
///////////////////////////////////////////////////////////////////////////////	
	void Module:: SetMix() 
	{ 
		if(! ((enabledOperations & 0x01) == 1))
			enabledOperations +=1; 
	}
	void Module:: RemoveMix()
	{
		if((enabledOperations & 0x01) == 1)
			enabledOperations -=1;
	}
	bool Module:: CanMix() { return (enabledOperations & 0x01) == 1;}
	
	
///////////////////////////////////////////////////////////////////////////////	
/*     																SPLIT		  												     */	
///////////////////////////////////////////////////////////////////////////////		
	void Module:: SetSplit() 
	{ 
		if(! ((enabledOperations & 0x02) == 2))
			enabledOperations +=2; 
	}
	void Module:: RemoveSplit()
	{
		if((enabledOperations & 0x02) == 2)
			enabledOperations -=2;
	}
	bool Module:: CanSplit() { return (enabledOperations & 0x02) == 2;}


///////////////////////////////////////////////////////////////////////////////	
/*     																HEAT		  												     */	
///////////////////////////////////////////////////////////////////////////////
	void Module:: SetHeat() 
	{ 
		if(! ((enabledOperations & 0x04) == 4))
			enabledOperations +=4; 
	}
	void Module:: RemoveHeat()
	{
		if((enabledOperations & 0x04) == 4)
			enabledOperations -=4;
	}
	bool Module:: CanHeat() { return (enabledOperations & 0x04) == 4;}

///////////////////////////////////////////////////////////////////////////////	
/*     															DETECT		  												     */	
///////////////////////////////////////////////////////////////////////////////	
	void Module:: SetDetect() 
	{ 
		if(! ((enabledOperations & 0x08) == 8))
			enabledOperations +=8; 
	}
	void Module:: RemoveDetect()
	{
		if((enabledOperations & 0x08) == 8)
			enabledOperations -=8;
	}
	bool Module:: CanDetect() { return (enabledOperations & 0x08) == 8;}
	
	
///////////////////////////////////////////////////////////////////////////////	
/*     																STORE		  												     */	
///////////////////////////////////////////////////////////////////////////////
	
	void Module:: SetStore(int i) 
	{ 
		numStorage=i;
		usedStorage=0;
		if(! ((enabledOperations & 0x10) == 16))
			enabledOperations +=16; 
	}
	void Module:: RemoveStore()
	{
	numStorage=0;
	usedStorage=0;
		if((enabledOperations & 0x10) == 16)
			enabledOperations -=16;
	}
	bool Module:: CanStore() { return (enabledOperations & 0x10) == 16;}
	
	int Module:: StorageCapacity() { return numStorage; }
	
	bool Module :: StoreNNodes(int n)
	{
		if ( n > numStorage - usedStorage)
			return false;
		usedStorage = n;
		return true;
	}
		
	
///////////////////////////////////////////////////////////////////////////////	
/*     																DISPENSE	 												     */	
///////////////////////////////////////////////////////////////////////////////
  void Module:: SetDispense() 
	{ 
		if(! ((enabledOperations & 0x20) == 32))
			enabledOperations +=32; 
	}
	void Module:: RemoveDispense()
	{
		if((enabledOperations & 0x20) == 32)
			enabledOperations -=32;
	}
	bool Module:: CanDispense() { return (enabledOperations & 0x20) == 32;}
	
	
///////////////////////////////////////////////////////////////////////////////	
/*     																OUTPUT	  												     */	
///////////////////////////////////////////////////////////////////////////////	
	void Module:: SetOutput() 
	{ 
		if(! ((enabledOperations & 0x40) == 64))
			enabledOperations +=64; 
	}
	void Module:: RemoveOutput()
	{
		if((enabledOperations & 0x40) == 64)
			enabledOperations -=64;
	}
	bool Module:: CanOutput() { return (enabledOperations & 0x40) == 64;}

	
