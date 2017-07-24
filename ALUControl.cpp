#include "ALUControl.h"
#include "Opcode.h"

ALUControl::ALUControl()
{

}
void ALUControl::printAll()
{
	cout << "ALUControl Unit: " <<endl;
	cout << "FunctionField: " << inFunctField << endl;
	if (opcodes.getName(inALUOp) == "add")
		{cout << "Input Type: RTYPE" << endl;}
	else{	cout << "Input Inst: " << opcodes.getName(inALUOp) <<endl;}
	cout << "Output Inst: " << outALUOperation << endl;

}
void ALUControl::calculate()
{

	
	if(opcodes.getOpcodeField(inALUOp) == "000000")
	{
		// ADD
		if(inFunctField == "0x20")
		{
			outALUOperation = "ADD";
		}
		// SUB
		else if(inFunctField == "0x22")
		{
			outALUOperation = "SUB"; 
		}
		//SLT
		else if(inFunctField == "0x2a")
		{
			outALUOperation = "LESSTHAN";
		}
	}
	// LW
	else if(opcodes.getOpcodeField(inALUOp)== "100011")
	{
		outALUOperation = "ADD";
	}
	// SW
	else if(opcodes.getOpcodeField(inALUOp)== "101011")
	{
		outALUOperation = "ADD";
	}
	//BEQ
	else if(opcodes.getOpcodeField(inALUOp)== "000100")
	{
		outALUOperation = "EQUAL";
	}
	// J
	else if(opcodes.getOpcodeField(inALUOp)== "000010")
	{
	}
	// ADDI
	else if(opcodes.getOpcodeField(inALUOp)== "001000")
	{
		outALUOperation = "ADD";
	}
} 
