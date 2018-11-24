//Name: Maxine Xin
#ifndef __BINARYPARSER_H__
#define __BINARYPARSER_H__

using namespace std;

#include <iostream>
#include <fstream>
#include "Instruction.h"
#include "OpcodeTable.h"
#include <vector>
#include <sstream>
#include <stdlib.h>

/*
 * This class reads in an file containing lines each of which is a single 32 
 * bit binary string, checks whether each binary string is correct in syntax
 * and format, and whether is a valid encoding of an ASM instruction. If yes,
 * this class will store that instruction in a list of instructions that can
 * be itereated through.
 */

class BinaryParser {
  public:
    // filename: string type name of the file
    // reads in a file containing lines of 32 bit binary string;
    // check the syntax correctness of each binary string and whether it is an
    // valid encoding of an ASM instruction;
    // if valid, decode the binary and store the corresponding instruction.
    BinaryParser(string filename);

    // return true if the input file is correct in syntax and has valid 
    // encodings of an ASM instruction;
    // otherwise, return false.
    bool isFormatCorrect() { 
      return myFormatCorrect; 
    };

    // Iterator that returns the next Instruction in the list of Instructions.
    Instruction getNextInstruction();

  private:
    vector<Instruction> myInstructions;    // list of instructions
    bool myFormatCorrect;                  // status representing the syntax
                                           // correctness and validity of a
                                           // binary string
    int myIndex;                           // iterator index
    OpcodeTable myOpTable;                 // encodings of all opcodes involved
    RegisterTable myRgstTable;             // names & number associated with MIPS registers
    const static int NUMREGISTERS = 32;           // number of registers
    const static int OPLEN = 6;                   // length of opcode field
    const static int RGSTLEN = 5;                 // length of register field
    const static int IMMRLEN = 5;                 // length of R type immediate field
    const static int IMMILEN = 16;                // length of I type immediate field
    const static int IMMJLEN = 26;                // length of J type immediate field
    const static int FUNCLEN = 6;                 // length of func field
    const static int ENCODELEN = 32;              // length of a valid encoding

    const string MULT_4 = "00";        // appends to the end of an encoding to *4
    const string FOREMOST_4_BITS_PC = "0000"; //4 most significant bits in PC             
    // binaryLine: the line of the binary string to be checked
    // returns true if that binary representation is correct in 
    // syntax(length, composed of 0's and 1's)
    bool checkSyntax(string binaryLine);
    
    // binaryLine: the line of the binary string to be encoded
    // returns true if that binary representation is a valid encoding of an ASM
    // instruction, stores value of each operand to each corresponding field of 
    // the instruction, and put the instruction to the instruction list;
    // returns false if binaryLine is not a valid encoding.
    bool decode2Inst(Instruction &i, string binaryLine); 
  
    // binaryLine: the line of the binary string(for a R type) to be encoded
    // op: the opcode of the instruction to be decoded into
    // i: the instruction that will hold the newly created instruction
    // returns false if encoding is invalid; otherwise, returns true and 
    // stores value of each operand to each corresponding field of 
    // the instruction based on the binary encoding passed in 
    // (might not be an valid R instruction at this time)
    bool decode2InstR(string binaryLine, Opcode op, Instruction &i);

    // binaryLine: the line of the binary string(for a I type) to be encoded
    // op: the opcode of the instruction to be decoded into
    // i: the instruction that will hold the newly created instruction
    // returns false if encoding is invalid; otherwise, returns true and
    // stores value of each operand to each corresponding field of 
    // the instruction based on the binary encoding passed in 
    // (might not be an valid I instruction at this time)
    bool decode2InstI(string binaryLine, Opcode op, Instruction &i);
 
    // binaryLine: the line of the binary string(for a J type) to be encoded
    // op: the opcode of the instruction to be decoded into
    // i: the instruction that will hold the newly created instruction
    // returns false if encoding is invalid; otherwise, returns true and
    // stores value of each operand to each corresponding field of 
    // the instruction based on the binary encoding passed in 
    // (might not be an valid J instruction at this time)
    bool decode2InstJ(string binaryLine, Opcode op, Instruction &i);
    
    // binaryStr: the binary string to be converted
    // returns an integer representing the decimal value of the binary string
    int cvtBinaryStr2Decimal(string binaryStr); 
   
    // immBinary: the binary string of an immediate value
    // returns a positive/negative integer representing the decimal value
    // of the binary string
    int cvtIMMBinary2Decimal(string immBinary);
 
    // i: instruction to be converted into ASM
    // returns the string of ASM that represents the instruction
    string cvt2ASM(Instruction &i);
 
    // operands: string array storing all operands based on their position
    // name: name of the instruction
    // isIMMLabel: boolean flag representing whether the imm field is an add label
    // returns the string representation of the R type ASM instruction
    string convert2RTypeASM(Opcode op, Instruction &i);

    // operands: string array storing all operands based on their position
    // name: name of the instruction
    // returns the string representation of the I type ASM instruction
    string convert2ITypeASM(Opcode op, Instruction &i);

    // operands: string array storing all operands based on their position
    // name: name of the instruction
    // returns the string representation of the J type ASM instruction
    string convert2JTypeASM(Opcode op, Instruction &i);

    // string: the immediate value to be sign extended
    // returns the string of the immediate value after sign extension
    string signExtendImm(string immStr);

    // string binaryLine: 32-bit encoding 
    // returns the Opcode representing the encoding
    Opcode getOpcodeFromBinary(string binaryLine); 
};



#endif
