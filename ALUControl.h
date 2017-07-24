#ifndef __ALUCONTROL_H__
#define __ALUCONTROL_H__

#include "Helper.h"
#include <string>
#include <vector>
/* This class creates an instance of the ALU control unit
 */
class ALUControl
{
 public:
	//create the control unit
  ALUControl();
  //Given an input, calculate all output.
  //This means given a control from Main Control Unit, and function field (bit 5 - 0 of instruction)
  //find an appropriate ALU math operation.
  //For example, if we know it is LW, then it should be ADD (add register value and imm value which is offset).
  //For example, if we know it is RTYPE, then look at functfield, so it knows what instruction it should give ALU to do
  void calculate();
  void printAll();
  // bunch of setter method
  void setInFunctField(string functField){inFunctField = functField;}
  void setInALUOp(Opcode ALUOp){inALUOp = ALUOp;}
  
  // bunch of getter method
  string getOutALUOperation(){return outALUOperation;}
  string getFunctField(){return inFunctField;}
 private:
  //input
  string inFunctField; //in binary form of length 6, e.g. "001100", "000000"
  Opcode inALUOp; //This will be "RTYPE" if the instruction is R-type. Otherwise, the control will actually be the name of instruction itself in uppercase, e.g. "BEQ", "LW". See Main Control Class for more information
  //output
  OpcodeTable opcodes; //encodings for opcodes 	
 string outALUOperation; //for ALU unit, so it is "SUB","ADD","EQUAL", or "LESSTHAN"
};

#endif
