// Name: Maxine Xin
#include "Pipeline.h"
#include "StallPipeline.h"
#include "ForwardingPipeline.h"
#include "ASMParser.h"
#include "BinaryParser.h"

#include <iostream>
#include <vector>
using namespace std;

/* This file reads in a file containing either lines of MIPS assembly
 * instructions or 32-bit binary encodings. If the file is correctly formatted,
 * the MIPS instructions / binary encodings will be converted to Instruction
 * instances, and the execution time for each instruction in ideal pipeline, 
 * stall pipeline, and forwarding pipeline will be computed and printed out to
 * stdout.
 *
 */

vector<Instruction> getInstFromASMFile(string filename) {
  vector<Instruction> instructions;
  Instruction i;

  ASMParser *aParser;
  aParser = new ASMParser(filename);

  if (aParser -> isFormatCorrect() == false) {
  cerr << "Format of input file is incorrect." << endl;
  exit(1);
  }

  i = aParser -> getNextInstruction();
  while (i.getOpcode() != UNDEFINED) {
    instructions.push_back(i);
    i = aParser -> getNextInstruction();
  }
  delete aParser;
  
  return instructions;
}

vector<Instruction> getInstFromBinaryFile(string filename) {
  vector<Instruction> instructions;
  Instruction i;

  BinaryParser *bParser;
  bParser = new BinaryParser(filename);
  if (bParser -> isFormatCorrect() == false) {
    cerr << "Format of input file is incorrect." << endl;
    exit(1);
  }
  i = bParser -> getNextInstruction();
  while (i.getOpcode() != UNDEFINED) {
    instructions.push_back(i);
    i = bParser -> getNextInstruction();
  }
  delete bParser;

  return instructions;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Need to specify a binary encoding / MIPS assembly instruction file"
      << " to translate." << endl;
    exit(1);
  }

  string filename = argv[1];
  vector<Instruction> instructions;

  if (filename.compare(filename.size() - 3, 3, "asm") == 0) {
    // if input file contains MIPS assembly instructions
    instructions = getInstFromASMFile(filename);
  } else {
    // if input file contains binary encodings
    instructions = getInstFromBinaryFile(filename);
  }

  //comment!!!!
  vector<Pipeline*> pipelinePtrs;

  Pipeline *idealPL = new Pipeline();
  StallPipeline *stallPL = new StallPipeline();
  ForwardingPipeline *forwardingPL = new ForwardingPipeline();

  pipelinePtrs.push_back(idealPL);
  pipelinePtrs.push_back(stallPL);
  pipelinePtrs.push_back(forwardingPL);

  for (unsigned int i = 0; i < pipelinePtrs.size(); i++) {
    pipelinePtrs[i] -> initialize(instructions);
    pipelinePtrs[i] -> execute();
    pipelinePtrs[i] -> print();  
    if (i != pipelinePtrs.size() - 1) 
      cout << endl;
  }
 
  for (unsigned int i = 0; i < pipelinePtrs.size(); i++) {
    delete pipelinePtrs[i];
  }
  
  return(0);
}

