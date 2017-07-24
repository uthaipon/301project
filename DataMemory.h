#ifndef __DATAMEMORY_H__
#define __DATAMEMORY_H__

#include "Helper.h"

/* This class creates an object that acts as an data memory unit
 */


class DataMemory
{
 public:
	//create the data memory with empty memory
  DataMemory();
  //create the data memory, with given database of data. The database is in the form string,string
  //for <address,data>
  //for example, "44578220" paired with "a7c31002"
  DataMemory(map<string,string> dataList);

  //given input and control, attempt to calculate the output, which is outReadData. This means it reads only if control for read is 1
  void read();
  //attempt to write. This means it writes only if control for write is 1. It writes writeData into inAddress
  void write();
  //print memory content
  void printMemoryContent();

  // setter methods
  void setInAddress(string address){inAddress = address;} //please add the method definitions for setter methods
  void setInWriteData(string writeData){inWriteData = writeData;}
  void setConMemRead(bool memReadVal){conMemRead = memReadVal;}
  void setConMemWrite(bool memWriteVal){conMemWrite = memWriteVal;}
  void setOutReadData(string readData){outReadData = readData;}
  void setData(map<string,string> dataList){mydataList = dataList;}
  // get output
  string getOutReadData(){return outReadData;}

  string getInAddress(){return  inAddress;} //please add the method definitions for setter methods
  string getInWriteData(){return inWriteData;}
  bool getConMemRead(){return conMemRead;}
  bool getConMemWrite(){return conMemWrite;}
  map<string,string> getData(){return mydataList;}

 private:
   //input
  string inAddress; //in hex form with no 0x, e.g. "0023ffff", "fffffffe"
  string inWriteData; //in hex form with no 0x, e.g. "0023ffff", "fffffffe"
  //control
  bool conMemRead;
  bool conMemWrite;
  //output
  string outReadData;
  
  map<string,string> mydataList;
};

#endif
