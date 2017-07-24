CC = g++ -std=gnu++0x 
LD = g++ -std=gnu++0x
CCFLAG = -Wall -Wno-deprecated -g -c
LDFLAG = -Wall -Wno-deprecated -g

EXECS = main Test
#in some case other platform create .exe and will not delete the executable, so I can add extraclean part if needed

all: $(EXECS)

main: main.o Counter.o InstMemory.o Helper.o RegisterMemory.o Instruction.o ASMParser.o Opcode.o RegisterTable.o MathUnit.o RegisterMemory.o DataMemory.o ALUControl.o MainControl.o

	$(LD) $(LDFLAG)  -o main main.o Counter.o InstMemory.o Helper.o Instruction.o ASMParser.o Opcode.o RegisterTable.o MathUnit.o RegisterMemory.o DataMemory.o ALUControl.o MainControl.o

	
main.o: main.cpp
	$(CC) $(CCFLAG) main.cpp
	
Test: Test.o Counter.o InstMemory.o Helper.o RegisterMemory.o Instruction.o ASMParser.o Opcode.o RegisterTable.o MathUnit.o RegisterMemory.o DataMemory.o ALUControl.o MainControl.o
	$(LD) $(LDFLAG) -o Test Test.o Counter.o InstMemory.o Helper.o Instruction.o ASMParser.o Opcode.o RegisterTable.o MathUnit.o RegisterMemory.o DataMemory.o ALUControl.o MainControl.o
	
Test.o: Test.h Test.cpp
	$(CC) $(CCFLAG) Test.cpp

Helper.o: Helper.h Helper.cpp 
	$(CC) $(CCFLAG) Helper.cpp
	
Counter.o: Counter.h Counter.cpp
	$(CC) $(CCFLAG) Counter.cpp

InstMemory.o: InstMemory.h InstMemory.cpp 
	$(CC) $(CCFLAG) InstMemory.cpp
	
RegisterMemory.o: RegisterMemory.h RegisterMemory.cpp 
	$(CC) $(CCFLAG) RegisterMemory.cpp

DataMemory.o: DataMemory.h DataMemory.cpp 
	$(CC) $(CCFLAG) DataMemory.cpp

Instruction.o: Instruction.h Instruction.cpp
	$(CC) $(CCFLAG) Instruction.cpp

ASMParser.o: ASMParser.h ASMParser.cpp
	$(CC) $(CCFLAG) ASMParser.cpp

Opcode.o: Opcode.h Opcode.cpp
	$(CC) $(CCFLAG) Opcode.cpp

RegisterTable.o: RegisterTable.h RegisterTable.cpp
	$(CC) $(CCFLAG) RegisterTable.cpp

MathUnit.o: MathUnit.h MathUnit.cpp
	$(CC) $(CCFLAG) MathUnit.cpp

ALUControl.o: ALUControl.h ALUControl.cpp
	$(CC) $(CCFLAG) ALUControl.cpp

MainControl.o: MainControl.h MainControl.cpp
	$(CC) $(CCFLAG) MainControl.cpp

clean:
	/bin/rm -f *.o $(EXECS)
