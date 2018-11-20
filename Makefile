# Name: Maxine Xin
# makefile
# compile and link all .h and .cpp files, and create executable

DEBUG_FLAG = -DDEBUG -g -Wall
CFLAGS = -DDEBUG -g  -std=c++11 -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<

PIPESIM: PipelineSimulation.o Pipeline.o StallPipeline.o ForwardingPipeline.o Instruction.o ASMParser.o BinaryParser.o DependencyChecker.o OpcodeTable.o RegisterTable.o
	g++ -o PIPESIM PipelineSimulation.o Pipeline.o StallPipeline.o ForwardingPipeline.o Instruction.o ASMParser.o BinaryParser.o DependencyChecker.o OpcodeTable.o RegisterTable.o

Pipeline.o: Pipeline.h Instruction.h OpcodeTable.h ASMParser.h BinaryParser.h RegisterTable.h DependencyChecker.h

StallPipeline.o: Pipeline.h

ForwardingPipeline.o: Pipeline.h

DependencyChecker.o: DependencyChecker.h OpcodeTable.h RegisterTable.h Instruction.h

ASMParser.o: ASMParser.h OpcodeTable.h RegisterTable.h Instruction.h

BinaryParser.o: BinaryParser.h OpcodeTable.h RegisterTable.h Instruction.h

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h

OpcodeTable.o: OpcodeTable.h

RegisterTable.o: RegisterTable.h

clean:
	/bin/rm -f ASM *.o core 
