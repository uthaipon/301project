#include "Counter.h"
#include "InstMemory.h"
#include "Helper.h"
#include "DataMemory.h"
#include "RegisterMemory.h"
#include "MathUnit.h"
#include "MainControl.h"
#include "ALUControl.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>


using namespace std;

/* The main method for simulating a non-pipeline process of intrustions. Require 5 classes that represents
	pieces of the arhitecture
*/

bool oneCycle();
void printAll();

//all units here
InstMemory instructionMemory;
RegisterMemory registerMemory;
DataMemory dataMemory;

Counter PC;	
MathUnit PCAdd;
MathUnit jumpSL2;
MathUnit branchSL2;
MathUnit branchAdd;
MathUnit writeMultiplexor;
MathUnit secondInputMultiplexor;
MathUnit signExtend;
MainControl control;
ALUControl ALUControlUnit;
MathUnit mainALU;
MathUnit branchMultiplexor;
MathUnit jumpMultiplexor;
MathUnit toWriteMultiplexor;

//17 units declared

//parameters

//CONFIGURATION FILE PARAMETERS

//provides the name of the input file containing MIPS assembly code.
string config_program_input;

//provides the name of the input file that contains the contents of the main memory module at the beginning of execution.
string config_memory_contents_input;

//provides the name of the input file that contains the contents of the register file at the beginning of execution.
string config_register_file_input;

//determines whether the user wishes to step through the simulation one instruction at a time, 
//or simply execute the simulation of the entire assembly program all at once.
enum outputMode {SINGLE_STEP, BATCH};
outputMode config_output_mode;

//If true, any debugging output should be displayed (whatever the programmer decides that debugging output is). 
//If false, no debugging output should be displayed.
bool config_debug_mode;

//If true, each output event should print out the current contents of the entire register file and the entirety of memory.
bool config_print_memory_contents;

int main(int argc, char *argv[])
{
	if(argc < 2){
		cerr << "Need to specify an input file which contains all the parameters."<<endl;
		exit(1);
	}
	
	//in reading input file, it should read the parameters in any order. For each parameter,
	//we can go through each line and find the line containing that parameter
	bool validConfig = true;
	ifstream configFile(argv[1]);
	configFile >> skipws;
	string line;

	if(configFile.is_open())
	{
		while(getline(configFile,line) && validConfig == true)
		{
			//cout << line << endl;
			if(!(line.size() == 0))
			{
				for(int i =0; i < (signed)line.size(); i++) //each character. sign to avoid the warning
				{
					if(line[i] == ' ') //delate white space or \t. Ignore them. We don't need them at all
					{
						line.erase(i,0);

					} 
					else if(line[i] == '\t') 
					{

						line.erase(i,0);
					}
				}

				if(!(line[0] == '#') ) //if line starts with #, ignore the whole line
				{
					int index = line.find('=');
		   			string parameter = line.substr(0,index); //left side
		   			string value = line.substr(index+1,line.size()-parameter.size()-1); //right side

					//check each parameter - what is it? 
					//Once known, just set the correct parameter accordingly to the value
		   			if (parameter == "program_input")
		   			{
		   				config_program_input = value;

		   			} else if (parameter == "memory_contents_input") {

		   				config_memory_contents_input = value;

		   			} else if (parameter == "register_file_input") {

		   				config_register_file_input = value;

		   			} else if (parameter == "output_mode") {
						//the two cases for output mode
		   				if(value.compare("single_step")==0)
		   				{

		   					config_output_mode = SINGLE_STEP;

		   				} else if (value.compare("batch")==0) {

		   					config_output_mode = BATCH;

		   				} else { //invalid out put mode. Print error

		   					validConfig = false;
		   					cout << value << endl;
		   					cout << value.size() << endl;
		   					cerr << "ERROR READING CONFIGURATION FILE: output_mode. output_mode must be single_step or batch" << endl;
		   					exit(1);

		   				}
		   			
		   			} else if (parameter == "debug_mode") {
					//the two cases for output mode
		   				if(value == "true")
		   				{
							config_debug_mode = true;

						} else if (value == "false") {

							config_debug_mode = false;

						} else {
							//invalid value mode. Print error
		   					validConfig = false;
		   					cerr << "ERROR READING CONFIGURATION FILE: debug_mode. debug_mode must be true or false" << endl;
		   					exit(1);
		   				}

		   			} else if (parameter == "print_memory_contents") {
						//the two cases for output mode
		   				if(value == "true")
		   				{
							config_print_memory_contents = true;

						} else if (value == "false") {

							config_print_memory_contents = false;

						} else {
							//invalid value. Print error
		   					validConfig = false;
		   					cerr << "ERROR READING CONFIGURATION FILE: print_memory_contents. print_memory_contents must be true or false" << endl;
		   					exit(1);

		   				}

		   			} else {
					//some unexpected parameter. Print error
		   				validConfig = false;
		   				cout << parameter << endl;
		   				cout << parameter.size() << endl;
		   				cerr << "ERROR READING CONFIGURATION FILE: INVALID PARAMETER. Valid parameters are: program_input, memory_contents_input, register_file_input, output_mode, debug_mode, print_memory_contents" << endl;
		   				exit(1);

		   			}

				}

			}	

		}
	}

	
	
	//setting up all the unit and initial value (if applicable)
	//start with all memory units
	//3 of them.
	instructionMemory.setInstructionList(Helper::readFileForInstruction(config_program_input));	
	registerMemory.setRegisterList(Helper::readFileForRegister(config_register_file_input));
	dataMemory.setData(Helper::readFileForDataMemory(config_memory_contents_input));
	
	//the rest
	//set up allowed operations for math units
	vector<string> addOps;
	addOps.push_back("ADD");
	
	vector<string> SL2Ops;
	SL2Ops.push_back("SL2");
	
	vector<string> SignExtendOps;
	SignExtendOps.push_back("SIGNEXTEND");
	
	vector<string> multiplexorOps;
	multiplexorOps.push_back("0");
	multiplexorOps.push_back("1");
	
	vector<string> mainALUUnitOps;
	mainALUUnitOps.push_back("ADD");
	mainALUUnitOps.push_back("SUB");
	mainALUUnitOps.push_back("EQUAL");
	mainALUUnitOps.push_back("LESSTHAN");
	
	//units in fetch stage
	PC.setNumber("0x400000"); //this is 0x400000
	PCAdd.setListOperation(addOps);
	PCAdd.setInNumber2("0x4"); //always add 4
	
	//in decode stage
	jumpSL2.setListOperation(SL2Ops);
	branchSL2.setListOperation(SL2Ops);
	branchAdd.setListOperation(addOps);
	writeMultiplexor.setListOperation(multiplexorOps);	
	secondInputMultiplexor.setListOperation(multiplexorOps);	
	signExtend.setListOperation(SignExtendOps);
	
	//execute
	mainALU.setListOperation(mainALUUnitOps);
	branchMultiplexor.setListOperation(multiplexorOps);
	jumpMultiplexor.setListOperation(multiplexorOps);
	
	//read
	//only datamemory is needed. Already defined
	
	//write
	toWriteMultiplexor.setListOperation(multiplexorOps);
	
	//total of 17 units are initialized	
	
	/*if (config_debug_mode) //this is for testing: I will see that everything works given a specific Dr Szejda input in the FIRST instruction only
	{
		//setting things up - PC should be correct
		//unit 1
		string PCNum = PC.getNumber();
		
		//step1: fetch
		
		//unit 2: PCAdd
		PCAdd.setInNumber1(PCNum);
		PCAdd.calculate();
		
		//unit 3:inst Memory
		instructionMemory.setAddress(PCNum);
		instructionMemory.calculate();
		
		string instructionBinaryThisCycle = instructionMemory.getOutInstruction();
		//now split into different segment as in the picture
		string instruction25To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(6,26),7);
		string instruction31To26Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(0,6),2);
		string instruction25To21Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(6,5),2);
		string instruction20To16Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(11,5),2);
		string instruction15To11Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(16,5),2);
		string instruction15To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(16,16),4);
		string instruction5To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(26,6),2);
		
		//decode stage
		
		//unit 4: shift left 2 for jum
		jumpSL2.setInNumber1(instruction25To0Hex);
		jumpSL2.calculate();
		
		//unit 5: main control
		control.setHexOpcode(instruction31To26Hex);
		control.calculate();
		
		//unit 6 : writeMultiplexor
		writeMultiplexor.setInNumber1(instruction20To16Hex);
		writeMultiplexor.setInNumber2(instruction15To11Hex);
		writeMultiplexor.setControl( Helper::boolToStr(control.getOutRegDst()) );
		writeMultiplexor.calculate();
		
		//unit 7: register memory
		//set everything that we can so far. So everything except write data
		registerMemory.setInReadRegister1(instruction25To21Hex);
		registerMemory.setInReadRegister2(instruction20To16Hex);
		registerMemory.setInWriteRegister(writeMultiplexor.getOutNumber());
		registerMemory.setConRegWrite(control.getOutRegWrite());
		registerMemory.read();
		
		//unit 8: sign extend
		signExtend.setInNumber1(instruction15To0Hex);
		signExtend.calculate();
		
		//unit 9: branchSL2
		branchSL2.setInNumber1(signExtend.getOutNumber());
		branchSL2.calculate();
		
		//unit 10: secondInputMultiplexor
		secondInputMultiplexor.setInNumber1(registerMemory.getOutReadData2());
		secondInputMultiplexor.setInNumber2(signExtend.getOutNumber());
		secondInputMultiplexor.setControl( Helper::boolToStr(control.getOutALUSrc()) );
		secondInputMultiplexor.calculate();
		
		//unit 11: branchAdd
		branchAdd.setInNumber1(PCAdd.getOutNumber());
		branchAdd.setInNumber2(branchSL2.getOutNumber());
		branchAdd.calculate();
		
		//unit 12:ALUControl
		ALUControlUnit.setInALUOp(control.getOutALUOp());
		ALUControlUnit.setInFunctField(instruction5To0Hex);
		ALUControlUnit.calculate();
		
		//step 3: execute
		
		//unit 13: mainALU
		mainALU.setInNumber1(registerMemory.getOutReadData1());
		mainALU.setInNumber2(secondInputMultiplexor.getOutNumber());
		mainALU.setControl(ALUControlUnit.getOutALUOperation());
		mainALU.calculate();
		//in picture, there are zero and ALU result. Let's follow that picture
		bool zero = (mainALU.getControl() == "ZERO") && (mainALU.getOutNumber() == "0x1" || mainALU.getOutNumber() == "1");
		//the ALU result is the normal output of mainALU
		
		//unit 14: branchMultiplexor
		branchMultiplexor.setInNumber1(PCAdd.getOutNumber());
		branchMultiplexor.setInNumber2(branchAdd.getOutNumber());
		branchMultiplexor.setControl( Helper::boolToStr(zero && control.getOutBranch()) ); //this is the ADD gate in the picture
		branchMultiplexor.calculate();
		
		//unit 15: jumpMultiplexor
		jumpMultiplexor.setInNumber1(branchMultiplexor.getOutNumber());
		jumpMultiplexor.setInNumber2("0x" + PCAdd.getOutNumber().substr(2,1) + jumpSL2.getOutNumber().substr(2,7) );
		//this is concatenating PC+4 and the jump address. Substr is used to get rid of "0x" in the front
		jumpMultiplexor.setControl( Helper::boolToStr(control.getOutJump()) );
		jumpMultiplexor.calculate();
		
		//execute is done enough to PC. Back to PC counter
		PC.setNumber(jumpMultiplexor.getOutNumber());
		
		//step 4: Read from memory
		
		//unit 16: DataMemory
		dataMemory.setInAddress(mainALU.getOutNumber());
		dataMemory.setInWriteData(registerMemory.getOutReadData2());
		dataMemory.setConMemRead(control.getOutMemRead());
		dataMemory.setConMemWrite(control.getOutMemWrite());
		//attempts to read and write, and will do only if control is set to read or write
		dataMemory.read();
		dataMemory.write();
		
		//step 5: write
		//unit 17: toWriteMultiplexor
		toWriteMultiplexor.setInNumber1(mainALU.getOutNumber());
		toWriteMultiplexor.setInNumber2(dataMemory.getOutReadData());
		toWriteMultiplexor.setControl( Helper::boolToStr(control.getOutMemtoReg()) );
		toWriteMultiplexor.calculate();
		
		//now potentially write back to register - back to unit 7
		registerMemory.setInWriteData(toWriteMultiplexor.getOutNumber());
		//other inputs and control are already set
		//attempts to write. Only write if control for write is 10
		registerMemory.write();
	}*/
	//then do each cycle.
	while (oneCycle() == true)
	{
		if (config_output_mode == SINGLE_STEP) //then we print after every single cycle
		{
			printAll();
		}
	}
	
	if (config_output_mode == BATCH) //then we should print at the end of the run
	{
		printAll();
	}
}

//perform one cycle of the whole architecture. It will return true if the cycle is performed succesfully - meaning the instruction
//is valid. It is not valid if given the PC value, the fetch instruction fails - e.g. the PC+4 is beyond the file
bool oneCycle()
{
	//every unit is similar - set up input, calculate, then output
	//setting things up - start with PC
		//unit 1
		string PCNum = PC.getNumber();
		cout << PCNum << endl;		
		//step1: fetch
		
		//unit 2: PCAdd
		PCAdd.setInNumber1(PCNum);
		PCAdd.calculate();
		
		//unit 3:inst Memory
		instructionMemory.setAddress(PCNum);
		instructionMemory.calculate();
		string instructionBinaryThisCycle = instructionMemory.getOutInstruction();
			
		//the case that the program ends or invalid
		if (instructionBinaryThisCycle == "")
		{
			return false; //finish the program. Done
		}
		
		
		//now split into different segment as in the picture.
		//The number x to y is exactly as in the picture (e.g. instruction [31-25] is 31 to 25)
		string instruction25To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(6,26),7);
		string instruction31To26Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(0,6),2);
		string instruction25To21Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(6,5),2);
		string instruction20To16Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(11,5),2);
		string instruction15To11Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(16,5),2);
		string instruction15To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(16,16),4);
		string instruction5To0Hex = Helper::binaryToHex(instructionBinaryThisCycle.substr(26,6),2);
		
		//decode stage
		
		//unit 4: shift left 2 for jum
		jumpSL2.setInNumber1(instruction25To0Hex);
		jumpSL2.calculate();
		
		//unit 5: main control
		control.setHexOpcode(instruction31To26Hex);
		control.calculate();
		
		//unit 6 : writeMultiplexor
		writeMultiplexor.setInNumber1(instruction20To16Hex);
		writeMultiplexor.setInNumber2(instruction15To11Hex);
		writeMultiplexor.setControl( Helper::boolToStr(control.getOutRegDst()) );
		writeMultiplexor.calculate();
		
		//unit 7: register memory
		//set everything that we can so far. So everything except write data
		registerMemory.setInReadRegister1(instruction25To21Hex);
		registerMemory.setInReadRegister2(instruction20To16Hex);
		registerMemory.setInWriteRegister(writeMultiplexor.getOutNumber());
		registerMemory.setConRegWrite(control.getOutRegWrite());
		//read stage. Write stage to come later
		registerMemory.read();
		
		//unit 8: sign extend
		signExtend.setInNumber1(instruction15To0Hex);
		signExtend.calculate();
		
		//unit 9: branchSL2
		branchSL2.setInNumber1(signExtend.getOutNumber());
		branchSL2.calculate();
		
		//unit 10: secondInputMultiplexor
		secondInputMultiplexor.setInNumber1(registerMemory.getOutReadData2());
		secondInputMultiplexor.setInNumber2(signExtend.getOutNumber());
		secondInputMultiplexor.setControl( Helper::boolToStr(control.getOutALUSrc()) );
		secondInputMultiplexor.calculate();
		
		//unit 11: branchAdd
		branchAdd.setInNumber1(PCAdd.getOutNumber());
		branchAdd.setInNumber2(branchSL2.getOutNumber());
		branchAdd.calculate();
		
		//unit 12:ALUControl
		ALUControlUnit.setInALUOp(control.getOutALUOp());
		//second part of input
		ALUControlUnit.setInFunctField(instruction5To0Hex);
		ALUControlUnit.calculate();
		
		//step 3: execute
		
		//unit 13: mainALU
		
		mainALU.setInNumber1(registerMemory.getOutReadData1());
		mainALU.setInNumber2(secondInputMultiplexor.getOutNumber());
		mainALU.setControl(ALUControlUnit.getOutALUOperation());
		mainALU.calculate();
		//in picture, there are zero and ALU result. Let's follow that picture
		bool zero = (mainALU.getControl() == "ZERO") && (mainALU.getOutNumber() == "0x1" || mainALU.getOutNumber() == "1");
		//the ALU result is the normal output of mainALU
		
		//unit 14: branchMultiplexor
		branchMultiplexor.setInNumber1(PCAdd.getOutNumber());
		branchMultiplexor.setInNumber2(branchAdd.getOutNumber());
		branchMultiplexor.setControl( Helper::boolToStr(zero && control.getOutBranch()) ); //this is the ADD gate in the picture
		branchMultiplexor.calculate();
		
		//unit 15: jumpMultiplexor
		jumpMultiplexor.setInNumber1(branchMultiplexor.getOutNumber());
		jumpMultiplexor.setInNumber2("0x" + PCAdd.getOutNumber().substr(2,1) + jumpSL2.getOutNumber().substr(2,7) );
		//this is concatenating PC+4 and the jump address. Substr is used to get rid of "0x" in the front
		jumpMultiplexor.setControl( Helper::boolToStr(control.getOutJump()) );
		jumpMultiplexor.calculate();
		
		//execute is done enough to PC. Back to PC counter
		string s1 = jumpMultiplexor.getOutNumber();
		//PC.setNumber("0x" + s1.substr(4,6));
		PC.setNumber(Helper::trimHex( jumpMultiplexor.getOutNumber() ));
		
		//step 4: Read from memory
		
		//unit 16: DataMemory
		dataMemory.setInAddress(mainALU.getOutNumber());
		dataMemory.setInWriteData(registerMemory.getOutReadData2());
		dataMemory.setConMemRead(control.getOutMemRead());
		dataMemory.setConMemWrite(control.getOutMemWrite());
		//attempts to read and write, and will do only if control is set to read or write
		dataMemory.read();
		dataMemory.write();
		
		//step 5: write
		//unit 17: toWriteMultiplexor
		toWriteMultiplexor.setInNumber1(mainALU.getOutNumber());
		toWriteMultiplexor.setInNumber2(dataMemory.getOutReadData());
		toWriteMultiplexor.setControl( Helper::boolToStr(control.getOutMemtoReg()) );
		toWriteMultiplexor.calculate();
		
		//now potentially write back to register - back to unit 7
		registerMemory.setInWriteData(toWriteMultiplexor.getOutNumber());
		//other inputs and control are already set
		//attempts to write. Only write if control for write is 10
		registerMemory.write();
		
		return true; //run successfully
}

void printAll()
{
	//start with printing PC
	cout << endl << "********** Printing all 17 units **********" << endl;
	//unit 1 - Counter 
	cout << "1. PC:" << endl;
	cout << "PC Number: " << PC.getNumber() << endl;
	cout << endl;
		
	//unit 2 - PC Adder
	cout << "2. PC Adder (for PC = PC+4):" << endl;
	PCAdd.printAll();
	cout << endl;
	
	//memory units
	//unit 3
	cout << "3. instructionMemory:" << endl;
	cout << "input: " << endl;
	cout << instructionMemory.getAddress() << endl;
	cout << "output: " << endl;
	//cout << instructionMemory.getOutInstruction() << endl;
	cout << Helper::binaryToHex(instructionMemory.getOutInstruction(),8) << endl;
	cout << "memory: " << endl;
	instructionMemory.printMemoryContent();
	cout << endl;
	
	//unit 4 - jumpSL2
	cout << "4. shift left 2 unit for jump address: " << endl;
	jumpSL2.printAll();
	cout << endl;
	
	//unit 5 - main control
	cout << "5. Main Control Unit:" << endl;
	control.printAll();
	cout << endl;
	
	//unit 6 - writeMultiplexor
	cout << "6. Multiplexor unit for write register: " << endl;
	writeMultiplexor.printAll();
	cout << endl;
	
	//unit 7
	cout << "7. RegisterMemory:" << endl;
	cout << "input: " << endl;
	cout << "read register1: " << registerMemory.getInReadRegister1() << endl;
	cout << "read register2: " << registerMemory.getInReadRegister2() << endl;
	cout << "write register: " << registerMemory.getInWriteRegister() << endl;
	cout << "write data: " << registerMemory.getInWriteData() << endl;
	cout << "control: " << endl;
	cout << "RegWrite: " << registerMemory.getConRegWrite() << endl;
	cout << "output: " << endl;
	cout << "read data1: " << registerMemory.getOutReadData1() << endl;
	cout << "read data2: " << registerMemory.getOutReadData2() << endl;
	cout << "memory: " << endl;
	registerMemory.printMemoryContent();
	cout << endl;
	
	//unit 8 - signExtend
	cout << "8. Sign Extension unit for imm field: " << endl;
	signExtend.printAll();
	cout << endl;	
	
	//unit 9 - branchSL2
	cout << "9. shift left 2 unit for branch address: " << endl;
	branchSL2.printAll();
	cout << endl;
	
	//unit 10 - secondInputMultiplexor
	cout << "10. Multiplexor unit for second input of the main ALU: " << endl;
	secondInputMultiplexor.printAll();
	cout << endl;
	
	//unit 11 - branchAdd
	cout << "11. ADD unit for getting branch address: " << endl;
	branchAdd.printAll();
	cout << endl;
		
	//unit 12 - ALU control
	cout << "12. ALU Control Unit:" << endl;
	ALUControlUnit.printAll();
	cout << endl;
	
	//unit 13 - main ALU Operation unit 
	cout << "13. Main ALU:" << endl;
	mainALU.printAll();
	cout << "Zero: " << ( (mainALU.getControl() == "ZERO") && (mainALU.getOutNumber() == "0x1" || mainALU.getOutNumber() == "1") ) << endl;
	cout << endl;
	
	//unit 14 - branchMultiplexor
	cout << "14. Multiplexor unit for choosing branch address or usual PC+4 address: " << endl;
	branchMultiplexor.printAll();
	cout << endl;
	
	//unit 15 - jumpMultiplexor
	cout << "15. Multiplexor unit for choosing jump address or not: " << endl;
	jumpMultiplexor.printAll();
	cout << endl;
	
	//unit 16
	cout << "16. dataMemory:" << endl;
	cout << "input: " << endl;
	cout << "address: " << dataMemory.getInAddress() << endl;
	cout << "write data: " << dataMemory.getInWriteData() << endl;
	cout << "control: " << endl;
	cout << "Memread: " << dataMemory.getConMemRead() << endl;
	cout << "Memwrite: " << dataMemory.getConMemWrite() << endl;	
	cout << "output: " << endl;
	cout << "read data: " << dataMemory.getOutReadData() << endl;
	cout << "memory: " << endl;
	dataMemory.printMemoryContent();
	cout << endl;
	
	//unit 17 - toWriteMultiplexor
	cout << "17. Multiplexor unit for choosing whether the data from ALU or data memory will be written back: " << endl;
	toWriteMultiplexor.printAll();
	cout << endl;	
}
