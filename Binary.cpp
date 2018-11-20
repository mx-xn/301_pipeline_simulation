//Name: Maxine Xin
#include "BinaryParser.h"
#include <iostream>
#include <iomanip>
using namespace std;

/* This file reads in a file containing lines of binary encodings at the 
 * command line. If the file has correct syntax and each encoding is valid,
 * the encodings will be translated into string of MIPS instructions and
 * printed out to stdout, one per line.
 *
 */

int main(int argc, char *argv[]) {
  BinaryParser *bParser;

  if (argc < 2) {
    cerr << "Need to specify a binary encoding file to translate." << endl;
    exit(1);
  }

  bParser = new BinaryParser(argv[1]);

  if (bParser -> isFormatCorrect() == false) {
    cerr << "Format of input file is incorrect." << endl;
    exit(1);
  }

  Instruction i;
 
  i = bParser -> getNextInstruction();
  while (i.getOpcode() != UNDEFINED) {
    cout << i.getEncoding() << "\t" << i.getASM() << endl;
    i = bParser -> getNextInstruction();
  }

  delete bParser;

}
