#ifndef __MAINCONTROL_H__
#define __MAINCONTROL_H__

#include "Helper.h"
#include <string>
#include <vector>
#include "Opcode.h"
#include "RegisterTable.h"
/* This class creates an instance of the main control unit
 */

class MainControl
{
 public:
	//create the control unit
  MainControl();
  //Given an input, calculate all output.
  //Note: if some output does not matter, then it should not change that output datafield
  //If it matters, then change it (to 0,1 or string for ALUOp line)
  void calculate();

  // bunch of setter method
  void setInOpcode(Opcode opcode){inOpcode = opcode;} //in binary form of length 6, e.g. "000000" is R type, or "010001"
  void setHexOpcode(string opcode){
//    cout << opcode << endl;
//    cout << Helper::hexToBinary(opcode).substr(2,6) << endl;
    inOpcode = opcodeTable.getOpcodeFromBin((Helper::hexToBinary(opcode)).substr(2,6)); hexOpcode = opcode;} 
 //output
  //All string output will be "RTYPE" for ALU, or if it is not, the name of operation in uppercase, e.g. "B", "LW", "ADDI", etc.
  //The motivation for this is that ALU control will receive this, and able to figure out exactly later 
  //what ALU operation should be.
  //For output sending signal to a multiplexor, string output control line will be "0" or "1"
  void setOutRegDst(bool outRegDstVal){outRegDst = outRegDstVal;} //to multiplexor, so it is string as "0" or "1"
  void setOutJump(bool outJumpVal){outJump = outJumpVal;}
  void setOutBranch(bool outBranchVal){outBranch = outBranchVal;}
  void setOutMemRead(bool outMemReadVal){outMemRead = outMemReadVal;}
  void setOutMemtoReg(bool outMemtoRegVal){outMemtoReg = outMemtoRegVal;} //to multiplexor
  void setOutALUOp(Opcode outAluOpVal){outALUOp = outAluOpVal;} //special one. See note above
  void setOutMemWrite(bool outMemWriteVal){outMemWrite = outMemWriteVal;}
  void setOutALUSrc(bool outALUSrcVal){outALUSrc = outALUSrcVal;}//to multiplexor
  void setOutRegWrite(bool outRegWriteVal){outRegWrite = outRegWriteVal;}

  void printAll(); 
  // bunch of getter method
  Opcode getInOpcode(){return inOpcode;}
  bool getOutRegDst(){return outRegDst;} 
  bool getOutJump(){return outJump;}
  bool getOutBranch(){return outBranch;}
  bool getOutMemRead(){return outMemRead;}
  bool getOutMemtoReg(){return outMemtoReg;} //to multiplexor
  Opcode getOutALUOp(){return outALUOp;} //special one. See note above
  bool getOutMemWrite(){return outMemWrite;}
  bool getOutALUSrc(){return outALUSrc;} //to multiplexor
  bool getOutRegWrite(){return outRegWrite;}
  string getHexOpcode(){return hexOpcode;}

 private:
  //input
  OpcodeTable opcodeTable;
  Opcode inOpcode; //in binary form of length 6, e.g. "000000" is R type, or "010001"
  string hexOpcode;
	//output
  //All string output will be "RTYPE" for ALU, or if it is not, the name of operation in uppercase, e.g. "B", "LW", "ADDI", etc.
  //The motivation for this is that ALU control will receive this, and able to figure out exactly later 
  //what ALU operation should be.
  //For output sending signal to a multiplexor, string output control line will be "0" or "1"
  bool outRegDst; 
  bool outJump;
  bool outBranch;
  bool outMemRead;
  bool outMemtoReg; //to multiplexor
  Opcode outALUOp; //special one. See note above
  bool outMemWrite;
  bool outALUSrc; //to multiplexor
  bool outRegWrite;
};

#endif
