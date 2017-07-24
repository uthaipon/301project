#include "DataMemory.h"
#include <string>
#include <map>

using namespace std;

DataMemory::DataMemory()
{

}

DataMemory::DataMemory(map<string, string> dataList)
{
	mydataList = dataList;
}

void DataMemory::read() 
{
	if(conMemRead == true)
	{
		outReadData = mydataList[inAddress];
	}
}

void DataMemory::write() 
{
	if(conMemWrite == true)
	{
		mydataList[inAddress] = inWriteData;
	}
}

void DataMemory::printMemoryContent()
{

	map<string, string>::iterator it;
	for ( it = mydataList.begin(); it != mydataList.end(); it++ )
	{
		cout << it->first << ":" << it->second << endl;
	}
}


