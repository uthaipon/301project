#include <string>      // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <fstream>      //std::ifstream
#include <map>			//std::map
#include "Helper.h"
#include "RegisterMemory.h"
using namespace std;

//create the register memory with empty memory
 RegisterMemory::RegisterMemory()
 {

 }
  //create the register memory, with given database of register. The database is in the form int,string
  //for example, "31" paired with "a7c31002". The int is from 0 to 31.
RegisterMemory::RegisterMemory(map<Register,string>  registerList)
{
	myRegisterList = registerList;
}

  //given read register 1 and 2, get the read data into the datafield
void RegisterMemory::read()
{
	outReadData1 = myRegisterList[inReadRegister1];
	outReadData2 = myRegisterList[inReadRegister2];
}
  //given write register and data, write to register ONLY IF the write control is 1
void RegisterMemory::write()
{
	if(conRegWrite == 1)
	{
		myRegisterList[inWriteRegister] = inWriteData;
	}
}

void RegisterMemory::printMemoryContent()
{
	for (int i = 0; i < 32; i ++)
	{
		cout << i << ":" << myRegisterList[i] << endl;
	}
}


