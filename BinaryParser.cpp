//Name: Maxine Xin
#include "BinaryParser.h"
#include "OpcodeTable.h"
#include <iomanip>

// filename: string type name of the file
// reads in a file containing lines of 32 bit binary string;
// check the syntax correctness of each binary string and whether it is an
// valid encoding of an ASM instruction;
// if valid, decode the binary and store the corresponding instruction.
BinaryParser::BinaryParser(string filename) {
  myFormatCorrect = true;
  Instruction i;

  ifstream in;
  in.open(filename.c_str());
  
  if (in.bad()) 
    myFormatCorrect = false;
  else {
    string line;
    while (getline(in, line)) {
      if (!checkSyntax(line)) {
        //if there is something wrong with the length or there are
        //other characters other than '0' and '1'
        myFormatCorrect = false;
        break;
      } 
   
      //if the syntax is correct, check whether is a valid binary representation
      //of an instruction, and decode and store instruction if valid
      if (!decode2Inst(i, line)) {
        myFormatCorrect = false;
        break;
      }

      string asmStr = cvt2ASM(i);
      i.setAssembly(asmStr);     
      i.setEncoding(line);
      myInstructions.push_back(i);
    }
  }
  myIndex = 0;
  return;
}

// Iterator that returns the next Instruction in the list of Instructions.
Instruction BinaryParser::getNextInstruction() {
  if (myIndex < (int)(myInstructions.size())) {
    myIndex++;
    return myInstructions[myIndex - 1];
  }
  Instruction i;
  return i;
}


// binaryLine: the line of the binary string to be checked
// returns true if that binary representation is correct in 
// syntax(length, composed of 0's and 1's)
bool BinaryParser::checkSyntax(string binaryLine) {
  if (binaryLine.length() != ENCODELEN) 
    return false;

  //if the length is correct, check whether only composed of 1 or 0
  for (int i = 0; i < ENCODELEN; i++) {
    if (binaryLine.at(i) != '0' && binaryLine.at(i) != '1')
      return false;
  }

  //length correct, and composed of only '0' or '1'
  return true;
}

// binaryLine: the line of the binary string to be encoded
// returns true if that binary representation is a valid encoding of an ASM
// instruction, and stores the corresponding instruction to the instruction list;
// returns false if binaryLine is not a valid encoding.
bool BinaryParser::decode2Inst(Instruction &i, string binaryLine) {
  //get Opcode
  Opcode opcode = getOpcodeFromBinary(binaryLine); 
  // pass in only the opfield and func filed 
 
  //if opcode is UNDEFINED
  if (opcode == UNDEFINED) {
    cout << "undefined" << endl;
    return false;
  }

  InstType instType = myOpTable.getInstType(opcode);

  if (instType == RTYPE) 
    return decode2InstR(binaryLine, opcode, i);
  else if (instType == ITYPE)
    return decode2InstI(binaryLine, opcode, i);
  else 
    return decode2InstJ(binaryLine, opcode, i);
}

// binaryLine: the line of the binary string to be encoded
// op: the opcode of the instruction to be decoded into
// i: the instruction that will hold the newly created instruction
// returns false if encoding is invalid; otherwise, returns true and
// creates an R type instruction based on the binary encoding passed in
// (might not be an valid R instruction at this time)
bool BinaryParser::decode2InstR(string binary, Opcode op, Instruction &i) {
  // get positions of each operand for the purpose of error checking
  int rs_p = myOpTable.RSposition(op);
  int rt_p = myOpTable.RTposition(op);
  int rd_p = myOpTable.RDposition(op);
  int imm_p = myOpTable.IMMposition(op);
  
  int rs, rt, rd, imm;
  int pos = OPLEN;
  
  rs = cvtBinaryStr2Decimal(binary.substr(pos, RGSTLEN));
  pos += RGSTLEN;
  rt = cvtBinaryStr2Decimal(binary.substr(pos, RGSTLEN));
  pos += RGSTLEN;
  rd = cvtBinaryStr2Decimal(binary.substr(pos, RGSTLEN));
  pos += RGSTLEN;
  imm = cvtBinaryStr2Decimal(binary.substr(pos, IMMRLEN));

  if (rs_p == -1) {
    if (rs != 0)
      return false;
    else 
      rs = NUMREGISTERS; 
  }

  if (rt_p == -1) {
    if (rt != 0)
      return false; 
    else 
      rt = NUMREGISTERS; 
  }

  if (rd_p == -1) {
    if (rd != 0)
      return false; 
    else 
      rd = NUMREGISTERS; 
  }

  if (imm_p == -1) {
    if (imm != 0)
      return false;
    else 
      imm = 0; 
  }

  i.setValues(op, rs, rt, rd, imm);

  return true;
}

// binaryLine: the line of the binary string to be encoded
// op: the opcode of the instruction to be decoded into
// i: the instruction that will hold the newly created instruction
// returns false if encoding is invalid; otherwise, returns true and
// creates an I type instruction based on the binary encoding passed in
// (might not be an valid I instruction at this time)
bool BinaryParser::decode2InstI(string binary, Opcode op, Instruction &i) {
  // get positions of each operand for the purpose of error checking
  int rs_p = myOpTable.RSposition(op);
  int rt_p = myOpTable.RTposition(op);
  int imm_p = myOpTable.IMMposition(op);
  
  int pos = OPLEN;
  int rs, rt, rd, imm;

  rs = cvtBinaryStr2Decimal(binary.substr(pos, RGSTLEN));
  pos += RGSTLEN;
  rt = cvtBinaryStr2Decimal(binary.substr(pos, RGSTLEN));
  pos += RGSTLEN;
  rd = NUMREGISTERS;
 
  string immStr = binary.substr(pos, IMMILEN);
  if (myOpTable.isIMMLabel(op)) {
    //if imm is a label
    immStr += "00";    //*4
  }
  signExtendImm(immStr);
  imm = cvtIMMBinary2Decimal(immStr);

  if (rs_p == -1) {
    if (rs != 0)
      return false; 
    else 
      rs = NUMREGISTERS;
  }

  if (rt_p == -1) {
    if (rt != 0)
      return false; 
    else 
      rt = NUMREGISTERS;
  }

  if (imm_p == -1) {
    if (imm != 0)
      return false;
    else 
      imm = 0;
  }
 
  i.setValues(op, rs, rt, rd, imm);

  return true;
}

// binaryLine: the line of the binary string to be encoded
// op: the opcode of the instruction to be decoded into
// i: the instruction that will hold the newly created instruction
// returns false if encoding is invalid; otherwise, returns true and
// creates an J type instruction based on the binary encoding passed in
// (might not be an valid J instruction at this time)
bool BinaryParser::decode2InstJ(string binary, Opcode op, Instruction &i) {
  int imm_p = myOpTable.IMMposition(op);

  int pos = OPLEN; 
  int rs, rt, rd, imm;

  rs = rt = rd = NUMREGISTERS;

  string immStr = binary.substr(pos, IMMJLEN);
  immStr += "00";             // *4
  immStr = "0000" + immStr;   // add 4 '0's to the front(the most significant 4 bits of PC)
  imm = cvtIMMBinary2Decimal(immStr); 

  if (imm_p == -1) {
    if (imm != 0)
      return false;
    else 
      imm = 0;
  }

  i.setValues(op, rs, rt, rd, imm);

  return true;
}

// binaryStr: the binary string to be converted
// returns an integer representing the decimal value of the binary string
int BinaryParser::cvtBinaryStr2Decimal(string binaryStr) {
  int decimal = 0;
  for (unsigned int i = 0; i < binaryStr.length(); i++) {
    decimal *= 2;
    if (binaryStr.at(i) == '1') 
      decimal += 1;
  }
  return decimal;
}

// immBinary: the binary string of an immediate value
// returns a positive/negative integer representing the decimal value
// of the binary string
int BinaryParser::cvtIMMBinary2Decimal(string immBinary) {
  bool isNeg = false;
  string zero = "0";
  string one = "1";
  if (immBinary.at(0) == '1')    //if negative
    isNeg = true;

  if (isNeg) {
    for (unsigned int i = 0; i < immBinary.length(); i++) {
      if (immBinary.at(i) == '0')
        immBinary.replace(i, 1, one); //convert '0' to '1'
      else 
        immBinary.replace(i, 1, zero); //convert '1' to '0'
    }
  }
  int immDecimal = cvtBinaryStr2Decimal(immBinary);
  
  if (isNeg) { 
    // plus 1 and then convert to negative
    immDecimal++;
    immDecimal -= 2 * immDecimal;
  }
  return immDecimal;
}

// i: instruction to be converted into ASM
// returns the string of ASM that represents the instruction
string BinaryParser::cvt2ASM(Instruction &i) {
  Opcode op = i.getOpcode();
  InstType type = myOpTable.getInstType(op);

  if (type == RTYPE) 
    return convert2RTypeASM(op, i);
  else if (type == ITYPE)
    return convert2ITypeASM(op, i);
  else //type == JTYPE
    return convert2JTypeASM(op, i);  
}


// operands: string array storing all operands based on their position
// name: name of the instruction
// isIMMLabel: boolean flag representing whether the imm field is an add label
// returns the string representation of the R type ASM instruction
string BinaryParser::convert2RTypeASM(Opcode op, Instruction &i) {
  string strASM = "";
  int numOperands = myOpTable.numOperands(op);
  vector<string> operands (numOperands);      // stores the string representation
                                              // for each operand based on their position
  
  string name = myOpTable.getInstName(op);
  int rs_p = myOpTable.RSposition(op);
  int rt_p = myOpTable.RTposition(op);
  int rd_p = myOpTable.RDposition(op);
  int imm_p = myOpTable.IMMposition(op);

  if (rs_p != -1)
    operands[rs_p] = myRgstTable.getName(i.getRS());
  if (rt_p != -1)
    operands[rt_p] = myRgstTable.getName(i.getRT());
  if (rd_p != -1)
    operands[rd_p] = myRgstTable.getName(i.getRD());
  if (imm_p != -1) {
    stringstream ss;
    ss << dec << i.getImmediate();
    operands[imm_p] = ss.str();
  }
  
  strASM += (name + "\t");

  for (unsigned int j = 0; j < operands.size() - 1; j++) 
    strASM += (operands[j] + ", ");
  strASM += operands[operands.size() - 1];  

  return strASM;
}

// operands: string array storing all operands based on their position
// name: name of the instruction
// returns the string representation of the I type ASM instruction
string BinaryParser::convert2ITypeASM(Opcode op, Instruction &i) {
  string strASM = "";
  int numOperands = myOpTable.numOperands(op);
  vector<string> operands (numOperands);      // stores the string representation
                                              // for each operand based on their position
  
  string name = myOpTable.getInstName(op);
  int rs_p = myOpTable.RSposition(op);
  int rt_p = myOpTable.RTposition(op);
  int imm_p = myOpTable.IMMposition(op);

  if (rs_p != -1)
    operands[rs_p] = myRgstTable.getName(i.getRS());
  if (rt_p != -1)
    operands[rt_p] = myRgstTable.getName(i.getRT());
  if (imm_p != -1) {
    stringstream ss;
    if (myOpTable.isIMMLabel(op)) {
      ss << "0x" << hex << i.getImmediate();
    } else {
      ss << dec << i.getImmediate();
    }
    operands[imm_p] = ss.str();
  }
  
  strASM += (name + "\t");

  if (myOpTable.isIMMMemory(op)) {
    // if expects memory address
    strASM += (operands[0] + ", " + operands[1] + "(" + operands[2] + ")");
  } else {
    for (unsigned int j = 0; j < operands.size() - 1; j++) {
      strASM += (operands[j] + ", ");
    }
    strASM += operands[operands.size() - 1];  
  }

  return strASM;
}

// operands: string array storing all operands based on their position
// name: name of the instruction
// returns the string representation of the J type ASM instruction
string BinaryParser::convert2JTypeASM(Opcode op, Instruction &i) {
  string strASM = "";
  
  string name = myOpTable.getInstName(op);

  string immStr = "";
  int imm_p = myOpTable.IMMposition(op);

  if (imm_p != -1) {
    stringstream ss;
    ss << "0x" << hex << i.getImmediate();
    immStr = ss.str();
  }
  
  strASM += (name + "\t" + immStr);

  return strASM;
}

// string: the immediate value to be sign extended
// returns the string of the immediate value after sign extension
string BinaryParser::signExtendImm(string immStr) {
  // append the first char of the string to the front until
  // the length reaches 32
  while (immStr.length() < ENCODELEN)
    immStr = immStr.at(0) + immStr;
  return immStr;
}

// string binaryLine: 32-bit encoding 
// returns the Opcode representing the encoding
Opcode BinaryParser::getOpcodeFromBinary(string binaryLine) {
  string opStr = binaryLine.substr(0, OPLEN);
  string funcStr = binaryLine.substr(OPLEN + RGSTLEN * 3 + IMMRLEN, FUNCLEN);

  return myOpTable.getOpcode(opStr, funcStr);
}
