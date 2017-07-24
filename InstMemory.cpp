#include "InstMemory.h"
#include <string>
#include <map>
#include "Instruction.h"

using namespace std;

InstMemory::InstMemory()
{
	startIndex = "0x400000";
}

InstMemory::InstMemory(map<string, Instruction> instructionList)
{

	myInstructionList = instructionList;
	startIndex = "0x400000";

}

void InstMemory::calculate()
{
	map<string,Instruction>::iterator it = myInstructionList.find(inAddress);
	if(it != myInstructionList.end())
	{
		outInstruction = myInstructionList[inAddress];
	} else {
		outInstruction = Instruction();
	}
}

void InstMemory::printMemoryContent()
{

	for (int i = 0; i < (signed)myInstructionList.size()*4; i =i+4)
	{
		cout << Helper::decToHex(Helper::hexToDec(startIndex) + i,6) << ":" << Helper::binaryToHex(myInstructionList[Helper::decToHex(Helper::hexToDec(startIndex) + i,6)].getEncoding(),8) << endl;
	}

}
