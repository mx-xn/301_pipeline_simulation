// Name: Maxine Xin
#include "Pipeline.h"

/* default constructor of the Pipeline class, 
 * sets all entries in myStageEntry to myStageEmpty
 * (indicating no stage occupied in the beginning)
 * 
 */
Pipeline::Pipeline() {
  myStageEmpty = -1;
  // set all entries to myStageEmpty 
  for (int st = 0; st < NUMSTAGES; st++) {
    myStageEntry[st] = myStageEmpty;
  }

  // set the name for this pipeline
  myName = "IDEAL";
}

/* destructor
 */
Pipeline::~Pipeline() {}

/* initializes the pipeline, stores the input instructions, and  
 * gets and stores all the dependences of the instructions
 *
 */
void Pipeline::initialize(vector<Instruction> instructions) {
  // go through the instructions, store them in the list,
  // and add them to myDepChecker 
  for (unsigned int i = 0; i < instructions.size(); i++) {
    myDepChecker.addInstruction(instructions[i]);
    myInstructions.push_back(instructions[i]);
  }
}

/* models processing instructions in ideal pipeline datapaths,
 * compute execution time for each instruction and store the time
 * into each Instruction instance
 *
 */
void Pipeline::execute() {
  list<Instruction>::iterator it; // keeps track of the inst in the last stage
  it = myInstructions.begin();   
    
  unsigned int instNum = 0;       // keeps track of the # of the inst to be fetched 
  int time = 0;

  // while loop will terminate when all instructions get processed and
  // the last stage is finished for the last instruction
  while (it != myInstructions.end()) {  
    for (int st = NUMSTAGES - 1; st >= 0; st--) {
      if (st == NUMSTAGES - 1 && myStageEntry[st] != myStageEmpty) {
        // if in the last stage and it is not empty,
        // removes the instruction in that stage and sets 
        // its execution time 
        (*it).setExeTime(time);
        it++;
        myStageEntry[NUMSTAGES - 1] = myStageEmpty;
      }

      // for each stage, if that stage is not currently occupied
      // get the instruction from the previous stage,
      // and sets the previous stage to empty;
      if (canMoveIntoStage(st)) {
        if (st == 0) {
          // if current stage if the first stage, and there is still instruction
          // to be fetched, fetch that instruction 
          if (instNum < myInstructions.size()) {
            myStageEntry[st] = instNum;
            instNum++;
          }
        } else {
          // if current stage is not the first stage
          myStageEntry[st] = myStageEntry[st - 1];
          myStageEntry[st - 1] = myStageEmpty;
        }
      }
    }
    time++;
  }
  return;
}

/* print out the RAW dependences among the list of instructions,
 * followed by each instruction and the corresponding execution time
 *
 */
void Pipeline::print() {
  cout << myName << ": " << endl;
  myDepChecker.printRAWDependences();      // ask myDepChecker to print out RAW dependences
  cout << "Instr# \t CompletionTime \t Mnemonic " << endl;
  list<Instruction>::iterator it;
  int instIndex = 0;
  int totalTime = 0;

  // goes through the instructions and prints out their execution times
  for (it = myInstructions.begin(); it != myInstructions.end(); it++) {
    cout << instIndex << "\t" 
         << (*it).getExeTime() << "\t\t\t" << "|"
         << (*it).getAssembly()
         << endl;
    instIndex++;
    totalTime = (*it).getExeTime();
  }
  cout << "Total time is " << totalTime << endl;
  
  return;
}

/* given a stage index, returns true if the next instruction can be processed
 * in that stage(meaning the stage is not currently occupied), 
 * returns false if otherwise
 *
 */
bool Pipeline::canMoveIntoStage(int st) {
  // returns whether the current stage is unoccupied
  return (myStageEntry[st] == myStageEmpty);
}
