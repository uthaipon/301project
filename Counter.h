#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "Helper.h"
#include <string>


/* This class creates an object that acts as a PC counter
 */


class Counter
{
 public:
	//create a counter instance with a starting number 0
  Counter();
  //create a counter instance with a starting number
  Counter(string number);

  //access what number it is now. The answer should be in string, for example, "400a28"
  string getNumber();
  //set the number
  void setNumber(string number);

  string decToHex(int num);
 private:
  string myNumber;
};

#endif
