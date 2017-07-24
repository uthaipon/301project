#ifndef __MATHUNIT_H__
#define __MATHUNIT_H__

#include "Helper.h"
#include <string>
#include <vector>
/* This class creates any unit that can compute a mathematical function given two inputs, and will output one number. The operation is determined by its control. Note that control for this is a string, not 0 or 1
   	It can create:
	- ALU that can do add, sub, less than, and equal. All these receive 2 inputs each of 32 bits, and output at most 32 bits
	- ALU that can only do add (so controls does not matter)
	- Multiplexor, whose input may be of different length from 5 to 32 bits (but it does not matter). The job is only to choose the correct input as an output
	- Sign extend. Extend from 16 bits into 32 bits. 
	- Shift left 2. Will not change the length of the input and output (so the two leftmost bits will get lost)
	For clarification:
	- If unit uses only 1 input, it would only use input1 and ignore input2
	- If unit cannot change the operation (e.g. always do ADD, nothing else), the constructor should limit the set of possible control to be only that operation, and initialize control value to that as well
	By default, set the inital control value to be the first value in the set of allowed controls. This will automatically
	- ADD,SUB will always output length 32. If the result has length beyond that, it means it is an overflow. The class checks this and print an error (cerr) when doing ADD or SUB, and ignore the overflow bit
 */

class MathUnit
{
 public:
	//create the math unit with the control initially set to "0", and controls can be set to anything

  //create the math unit with restricted possible math operations. For example, if it's multiplexor, we want the operation to be only f(x1,x2)=x1 or f(x1,x2)=x2, and no other operation, so listOperation = {"0","1"}. Choose the initial value of control to be the first of the given set of allowed controls
  //Control are {"0","1","ADD","SUB","EQUAL","LESSTHAN","SL2","SIGNEXTEND"}
  //Represent: f(x1,x2)={x1,x2,x1+x2,x1-x2,(x1==x2) [is 1 if x1=x2, and 0 otherwise],(x1<x2) [is 1 if x1<x2, and 0 otherwise, two-complement is considered],x1<<2 [ignore x2], [sign-extend x1 from 16 bits to 32 bits, and ignore x2]} respectively
  MathUnit();

  //with the current (at most) 2 inputs and control, do the operation and put result into the output. This does not change input or control the control determines the operation, so the algorithms is a lot of cases (many if-else checking control value)
  void calculate();
  //print input, output, and control value. It will NOT print control value if the number of operations allowed is 1 (since we know what operation is already)
  void printAll();

  // bunch of setter method
  void setInNumber1(string num){inNumber1 = num;}
  void setInNumber2(string num){inNumber2 = num;} 
  void setControl(string controlVal){control = controlVal;}
  void setListOperation(vector<string> listOperation);
  // bunch of getter method
  string getOutNumber(){return outNumber;} 
  string getInNumber1(){return inNumber1;}
  string getInNumber2(){return inNumber2;}
  string getControl(){return control;}
 private:
  //input
  //in hex form, e.g. "0000001a" = 16+10= register 26. Note that there's two complement: "ffffffff" is -1, for example
  //also, different function may use different length of inputs, see comment of the class above
  string inNumber1; 
  string inNumber2;
  //control as a string. 
  //It can be string, e.g. "ADD", "SUB", "0", "1". See comment of the class and constructor above
  string control;
  //output
  string outNumber;
  vector<string> listOperationAllowed;
};

#endif
