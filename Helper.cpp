#include "Helper.h"
using namespace std;


//given a number, convert to hexadecimal. If the number is negative, do two complements of length 32 binary
 //which is length 8 hexadecimal
string Helper::decToHex(int number, int length)
{
	stringstream getHex;
	if (length == -1)
	{
		getHex << "0x" << setfill('0') << setw(8) << hex << number;
	} else {
    	getHex << "0x" << setfill('0') << setw(length) << hex << number;
	}
    return getHex.str();
}

int Helper::hexToDec(string hexRep)
{
	stringstream getHex;
    getHex << dec << stol(hexRep,NULL,16); //see std::stoi, stoi converts hexRep string to integer hex
    return stol(getHex.str()); //std::stoi default base is 10
}
string Helper::hexToBinary(string hexrep)
{
    stringstream ss;
    for (int i = 2; i < (signed)hexrep.length(); i++)
	{
		if(hexrep[i] == '0')
		{
			ss << "0000";
		}
		else if(hexrep[i] == '1')
		{
			ss << "0001";
		}
		else if(hexrep[i] == '2')
		{
			ss << "0010";
		}
		else if(hexrep[i] == '3')
		{
			ss << "0011";
		}
		else if(hexrep[i] == '4')
		{
			ss << "0100";
		}
		else if(hexrep[i] == '5')
		{
			ss << "0101";
		}
		else if(hexrep[i] == '6')
		{
			ss << "0110";
		}
		else if(hexrep[i] == '7')
		{
			ss << "0111";
		}
		else if(hexrep[i] == '8')
		{
			ss << "1000";
		}
		else if(hexrep[i] == '9')
		{
			ss << "1001";
		}
		else if(hexrep[i] == 'a')
		{
			ss << "1010";
		}
		else if(hexrep[i] == 'b')
		{
			ss << "1011";
		}
		else if(hexrep[i] == 'c')
		{
			ss << "1100";
		}
		else if(hexrep[i] == 'd')
		{
			ss << "1101";
		}
		else if(hexrep[i] == 'e')
		{
			ss << "1110";
		}
		else if(hexrep[i] == 'f')
		{
			ss << "1111";
		}

	}
	return ss.str();
}

string Helper::boolToStr(bool val)
{
	if (val == true)
	{
		return "1";
	}
	else
	{
		return "0";
	}

}
string Helper::binaryToHex(string binary, int length)
{
	stringstream getHex;
	if(length == -1)
	{
		getHex << "0x" << setfill('0') << setw(32) << hex << stol(binary,NULL,2); //see std::stoi, stoi converts hexRep string to integer hex
	} else {
		getHex << "0x" << setfill('0') << setw(length) << hex << stol(binary,NULL,2); //see std::stoi, stoi converts hexRep string to integer hex
	}
    
    return getHex.str(); //std::stoi default base is 10
}

/**string Helper::hexToBinary(string hexString, int length)
{

}**/

map<string, Instruction>  Helper::readFileForInstruction(string filename)
{
	map<string,Instruction> instructionMap;
	ASMParser *parser = new ASMParser(filename);
	string index = "0x400000";

  	if(parser->isFormatCorrect() == false)
  	{

    	cerr << "Format of input file is incorrect " << endl;
    	exit(1);

  	}

  	Instruction i;

  	//Iterate through instructions, printing each encoding.
  	i = parser->getNextInstruction();

  	while( i.getOpcode() != UNDEFINED)
  	{
    	instructionMap[index]=i;
    	//:cout << index << " " << i.getEncoding() << endl;
    	stringstream updateIndex;
    	updateIndex << showbase << hex << (hexToDec(index)+4);;
    	index = updateIndex.str();
    	i = parser->getNextInstruction();
  	}
  
  	delete parser;

	return instructionMap;
}

//given a file name which contains two parameters in each line in the form "0xppp:0xqqq", breakdown into
//a map from "ppp" to "qqq"
map<string,string> Helper::readFileForDataMemory(string filename)
{
	ifstream infile(filename.c_str());
	string line;
	map<string,string> memoryMap;

	while (getline(infile, line))
	{
	   int index = line.find(':');
	   string key = line.substr(0,index);
	   string data = line.substr(index+1,line.size()-1);
//	   cout << key << data << endl;
	   memoryMap[key] = data;
	}

	return memoryMap;
}

//given a file name which contains two parameters in each line in the form "[register number]:0xqqq", breakdown into
//a map from int registerNumber to "qqq"
map<Register,string> Helper::readFileForRegister(string filename)
{
	ifstream infile(filename.c_str());
	string line;
	RegisterTable registers;
	map<Register,string> registerMap;

	while (getline(infile, line))
	{
	   int index = line.find(':');
	   string key = line.substr(0,index);
	   string data = line.substr(index+1,line.size()-1);
	   Register reg = registers.getNum("$"+key);
	   //cout << reg << data << endl;
	   registerMap[reg] = data;
	}

	return registerMap;
}

string Helper::trimHex(string hexString){
	int indexOfFirstNonZero;
	bool hitNonZero = false;
	int index = 2;
	while(index < (signed)hexString.size() && hitNonZero ==false)
	{
		if(hexString[index] != '0')
		{
			indexOfFirstNonZero = index;
			hitNonZero = true;
		} else {
			index++;
		}
	}
	return "0x" + hexString.substr(indexOfFirstNonZero, hexString.size()-indexOfFirstNonZero);
}


