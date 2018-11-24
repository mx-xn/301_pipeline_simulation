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

// This methods reads in a filename, and call the ASMParser to check the syntax
// correctness of that file(containing MIPS assembly instructions); 
// If the file is syntactically correct, will return a vector of instructions
// associated with the read in file; otherwise, an error message will be
// printed out
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

// This methods reads in a filename, and call the BinaryParser to check the 
// syntax correctness of that file(containing MIPS assembly instruction
// encodings);
// If the file is syntactically correct, will return a vector of instructions
// associated with the read in file; otherwise, an error message will be
// printed out
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

  // this vector will store 3 pointers, each of which points to 
  // a instance of Pipeline class; more specifically, one of them 
  // belongs to Pipeline class, another belongs to both StallPipeline and
  // Pipeline class, and the rest belongs to both ForwardingPipeline and
  // Pipeline class.
  // (StallPipeline and ForwardingPipeline inherites Pipeline class)
  vector<Pipeline*> pipelinePtrs;

  // call each constructor
  Pipeline *idealPL = new Pipeline();
  StallPipeline *stallPL = new StallPipeline();
  ForwardingPipeline *forwardingPL = new ForwardingPipeline();

  // push each pointer to the vector
  pipelinePtrs.push_back(idealPL);
  pipelinePtrs.push_back(stallPL);
  pipelinePtrs.push_back(forwardingPL);

  // iterates through the vector of pointers, and simulates running the
  // instructions in each pipelines, and print out the output
  for (unsigned int i = 0; i < pipelinePtrs.size(); i++) {
    pipelinePtrs[i] -> initialize(instructions);
    pipelinePtrs[i] -> execute();
    pipelinePtrs[i] -> print();  
    if (i != pipelinePtrs.size() - 1) 
      cout << endl;
  }

  // delete the pointers
  for (unsigned int i = 0; i < pipelinePtrs.size(); i++) {
    delete pipelinePtrs[i];
  }
  
  return(0);
}

