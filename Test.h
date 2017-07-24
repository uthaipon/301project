#ifndef __TEST_H__
#define __TEST_H__

#include "Helper.h"
#include <string>

/* This class tests all the 5 classes and the helper method
 */
class Test
{
 public:
	Test();
 //each method will test all classes. Test helper, and then test each of 5 classes used to create a unit.
 //if the test passes, the end of method will print "Pass Test"
	static void testHelper();
	//test 5 classes
	static void testCounter();
	static void testInstMemory();
	static void testRegisterMemory();
	static void testDataMemory();
	static void testMathUnit();
	static void testMain();
	void testALUControl();

private:
	OpcodeTable opcodes;
};

#endif
