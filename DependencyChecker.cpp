#include "DependencyChecker.h"

DependencyChecker::DependencyChecker(int numRegisters)
/* Creates RegisterInfo entries for each of the 32 registers and creates lists for
 * dependencies and instructions.
 */
{
  RegisterInfo r;

  // Create entries for all registers
  for(int i = 0; i < numRegisters; i++){
    myCurrentState.insert(make_pair(i, r));
  }
}

void DependencyChecker::addInstruction(Instruction i)
/* Adds an instruction to the list of instructions and checks to see if that 
 * instruction results in any new data dependencies.  If new data dependencies
 * are created with the addition of this instruction, appropriate entries
 * are added to the list of dependences.
 */
{
  InstType iType = i.getInstType();
  Opcode op = i.getOpcode();       //get opcode

  switch(iType){
  case RTYPE:
    // get rs and rt and check Read Dependence if it exists
    if (myOpcodeTable.RSposition(op) != -1) {
      unsigned int rs = i.getRS();
      checkForReadDependence(rs);
    }
    if (myOpcodeTable.RTposition(op) != -1) {
      unsigned int rt = i.getRT();
      checkForReadDependence(rt);
    }
    //get rd and check Write Dependence if it exists
    if (myOpcodeTable.RDposition(op) != -1) {
      unsigned int rd = i.getRD();
      checkForWriteDependence(rd);   
    }
    break;
  case ITYPE:
    // get rs and check Read Dependence if it exists
    if (myOpcodeTable.RSposition(op) != -1) {
      unsigned int rs = i.getRS();
      checkForReadDependence(rs);
    }
    // get rd and check Write Dependence if it exists
    if (myOpcodeTable.RTposition(op) != -1) {
      unsigned int rt = i.getRT();
      checkForWriteDependence(rt);
    }
    break;
  case JTYPE: 
    // nothing needs to be done here
    break;
  default:
    // nothing needs to be done here
    break;
  }

  // add instruction to Instruction list
  myInstructions.push_back(i);

}

void DependencyChecker::checkForReadDependence(unsigned int reg)
/* Determines if a read data dependence occurs when reg is read by the current
 * instruction.  If so, adds an entry to the list of dependences. Also updates
 * the appropriate RegisterInfo entry regardless of dependence detection.
 */
{
  RegisterInfo regInfoPrev = myCurrentState.at(reg);  //the latest info of reg
  // we do not care about RAR, so just check for RAW, and
  // we add the Dependence to the dependence list if there is such a RAW dependence
  if (regInfoPrev.accessType == WRITE) {
    Dependence dp;
    dp.dependenceType = RAW;
    dp.registerNumber = reg;
    dp.previousInstructionNumber = regInfoPrev.lastInstructionToAccess;
    dp.currentInstructionNumber = myInstructions.size();
    myDependences.push_back(dp);
  } 
  // construct a value, and update the value corresponding to the key for 
  // my current state
  RegisterInfo regInfo;
  regInfo.lastInstructionToAccess = myInstructions.size();
  regInfo.accessType = READ;  
  myCurrentState[reg] = regInfo;
}


void DependencyChecker::checkForWriteDependence(unsigned int reg)
/* Determines if a write data dependence occurs when reg is written by the current
 * instruction.  If so, adds an entry to the list of dependences. Also updates 
 * the appropriate RegisterInfo entry regardless of dependence detection.
 */
{
  
  RegisterInfo regInfoPrev = myCurrentState.at(reg);  //the latest info of reg

  // check for WAR and WAW, and
  // we add the Dependence to the dependence list if there is such 
  // a WAR/WAW dependence
  if (regInfoPrev.accessType != A_UNDEFINED) {
    Dependence dp;
    if (regInfoPrev.accessType == WRITE)
      dp.dependenceType = WAW;
    else 
      dp.dependenceType = WAR;
    dp.registerNumber = reg;
    dp.previousInstructionNumber = regInfoPrev.lastInstructionToAccess;
    dp.currentInstructionNumber = myInstructions.size();
    myDependences.push_back(dp); 
  }
 
  // construct a value, and update the value corresponding to the key for 
  // my current state
  RegisterInfo regInfo;
  regInfo.lastInstructionToAccess = myInstructions.size();
  regInfo.accessType = WRITE;  
  myCurrentState[reg] = regInfo;
}

bool DependencyChecker::isRAW(int i1Num, int i2Num)
/* Given two instruction numbers i1Num and i2Num, check whether there is a RAW dependence 
 * between instruction i1 and i2(more specifically, whether the read register of i2 is
 * the same as the write register of i1).
 * Returns true is there is a RAW, and returns false if otherwise.
 */ 
{
  // iterate through the list of RAW instructions, return true if find a match,
  // otherwise return false
  list<Dependence>::iterator diter;
  for (diter = myDependences.begin(); diter != myDependences.end(); diter++) {
    if ((*diter).dependenceType == RAW              &&
        (*diter).previousInstructionNumber == i1Num && 
        (*diter).currentInstructionNumber  == i2Num   ) {
      return true;
    } 
  }
  return false;
}

void DependencyChecker::printRAWDependences() 
/* Prints out the RAW dependences followed by the correponding instructions 
 */ 
{ 
  list<Dependence>::iterator depIter;
  for (depIter = myDependences.begin(); depIter != myDependences.end(); depIter++) {
    if ((*depIter).dependenceType == RAW) {
      cout << "RAW Dependence between instruction ";
      list<Instruction>::iterator instIter = myInstructions.begin();
      int i1Num = (*depIter).previousInstructionNumber;
      int i2Num = (*depIter).currentInstructionNumber;
      
      advance(instIter, i1Num);
      cout << i1Num << " " << (*instIter).getAssembly() << " and ";
      advance(instIter, i2Num - i1Num);
      cout << i2Num << " " << (*instIter).getAssembly() << endl;
    }

    // cout << i1Num << " and " << i2Num << endl;
  } 
}

void DependencyChecker::printDependences()
/* Prints out the sequence of instructions followed by the sequence of data
 * dependencies.
 */ 
{
  // First, print all instructions
  list<Instruction>::iterator liter;
  int i = 0;
  cout << "INSTRUCTIONS:" << endl;
  for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++){
    cout << i << ": " << (*liter).getAssembly() << endl;
    i++;
  }

  // Second, print all dependences
  list<Dependence>::iterator diter;
  cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
  for(diter = myDependences.begin(); diter != myDependences.end(); diter++){
    switch( (*diter).dependenceType){
    case RAW:
      cout << "RAW \t";
      break;
    case WAR:
      cout << "WAR \t";
      break;
    case WAW:
      cout << "WAW \t";
      break;
    default:
      break;
    }

    cout << "$" << (*diter).registerNumber << " \t";
    cout << "(" << (*diter).previousInstructionNumber << ", ";
    cout << (*diter).currentInstructionNumber << ")" << endl;
  }


}
