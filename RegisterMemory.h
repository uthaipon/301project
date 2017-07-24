#ifndef __REGISTERMEMORY_H__
#define __REGISTERMEMORY_H__

#include "Helper.h"
#include <string>
#include <map>
/* This class creates an object that acts as a Register file. It has a memory, which is the
	content of 32 registers, and a read/write input and output
 */

class RegisterMemory
{
 public:
	//create the register memory with empty memory
  RegisterMemory();
  //create the register memory, with given database of register. The database is in the form int,string
  //for example, "31" paired with "a7c31002". The int is from 0 to 31.
  RegisterMemory(map<Register,string> registerList);

  //given read register 1 and 2, get the read data into the datafield
  void read();
  //given write register and data, write to register ONLY IF the write control is 1
  void write();
  //print content
  void printMemoryContent();

  // bunch of setter method
  void setInReadRegister1(string reg){inReadRegister1 = (Register) Helper::hexToDec(reg);} //reg is in hex form
  void setInReadRegister2(string reg){inReadRegister2 = (Register) Helper::hexToDec(reg);} //reg is in hex form
  void setInWriteRegister(string reg){inWriteRegister = (Register) Helper::hexToDec(reg);} //reg is in hex form
  void setInWriteData(string data){inWriteData = data;} 
  void setConRegWrite(bool controlVal){conRegWrite = controlVal;}
  void setRegisterList(map<Register, string> registerList){myRegisterList = registerList;} 
  // bunch of getter method
  string getOutReadData1(){return outReadData1;}
  string getOutReadData2(){return outReadData2;}	  
  Register getInReadRegister1(){return inReadRegister1;}
  Register getInReadRegister2(){return inReadRegister2;}
  Register getInWriteRegister(){ return inWriteRegister;}
  map<Register,string> getMyRegisterMemory(){return myRegisterList;}
  bool getConRegWrite(){return conRegWrite;}
  string getInWriteData(){return inWriteData;}

 private:
   //input
  Register inReadRegister1; //in hex form, e.g. "1a" = 16+10= register 26
  Register inReadRegister2;
  Register inWriteRegister;
  string inWriteData; //e.g. "32b004f"
  //control
  bool conRegWrite; //control - is either 1 or 0. If 1, write when called actually will write
  //output
  string outReadData1;
  string outReadData2;
  
  map<Register,string> myRegisterList;
};

#endif
