#include "Counter.h"
#include "Helper.h"

using namespace std;

//default - create a counter instance with a starting number 0
Counter::Counter()
{
	myNumber = "0x0";
}

  //create a counter instance with a starting number
Counter::Counter(string number)
{
	myNumber = number;
}

  //access what number it is now
string Counter::getNumber()
{
	return myNumber;
}





//for given Counter, decode into string
void Counter::setNumber(string number)
{
	myNumber = number;
}
