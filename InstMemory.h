#ifndef __INSTMEMORY_H__
#define __INSTMEMORY_H__

#include "Helper.h"
#include "Instruction.h"

/* This class creates an object that acts as a PC counter
 */


class InstMemory
{
 public:
	//create the instruction memory with empty memory
  InstMemory();
  //create the instruction memory, with given database of instruction. The database is in the form string,string
  //for example, "44578220" paired with "a7c31002"
  InstMemory(map<string,Instruction> instructionList);

  //given input, calculate the output
  void calculate();
  //print memory content
  void printMemoryContent();

  // getters/setters
  void setAddress(string hexAddress){inAddress = hexAddress;}
  void setInstructionList(map<string,Instruction> instructionList){myInstructionList = instructionList;}
  void setOutInstruction(Instruction instruction){outInstruction = instruction;}

  string getAddress(){return inAddress;} 
  map<string, Instruction> getInstructionMemory(){return myInstructionList;}
  string getOutInstruction(){ 
    if( outInstruction.getOpcode() == UNDEFINED) {
      return "";
    } else { 
      return outInstruction.getEncoding();
    }
  };

 private:
   //input
  string inAddress;
  //output
  Instruction outInstruction;
  
  map<string, Instruction> myInstructionList;

  string startIndex;
};

#endif
