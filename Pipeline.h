// Name: Maxine Xin
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include <vector>

#include "DependencyChecker.h"
#include "Instruction.h"
#include "OpcodeTable.h"
#include "RegisterTable.h"

using namespace std;

// enum of each stage
enum Stage {
  FETCH,
  DECODE,
  EXECUTE,
  MEMORY,
  WRITEBACK,
  NUMSTAGES 
};

/* This class models the Pipeline. Given a list of instances of Instructions,
 * the Pipeline class models processing each instruction in an ideal Pipeline
 * (ignoring all stalls), and compute execution time for each instruction.
 *
 */
class Pipeline {
  public:

    /* default constructor of the Pipeline class,
     * sets all entries in myStageEntry to myStageEmpty, 
     * (indicating no stage occupied in the beginning)
     */
    Pipeline();

    /* destructor
     */
    virtual ~Pipeline(); 

    /* initializes the pipeline, stores the input instructions, and 
     * gets and stores all the dependences of the instructions
     *
     */
    void initialize(vector<Instruction> instructions);

    /* models processing instructions in ideal pipeline datapaths,
     * compute execution time for each instruction and store the time
     * into each Instruction instance
     *
     */
    void execute();

    /* print out the RAW dependences among the list of instructions,
     * followed by each instruction and the corresponding execution time
     *
     */
    void print();

  private:

    /* given a Stage index, returns true if the next instruction can be processed
     * in that stage(meaning the stage is not currently occupied for IDEAL pipeline), 
     * returns false if otherwise
     *
     */
    virtual bool canMoveIntoStage(int st);
    

  protected:
    string myName;                     // name of the pipeline 
    list<Instruction> myInstructions;  // stores instructions
    DependencyChecker myDepChecker;    // stores dependences
    OpcodeTable myOpcodeTable;         // stores info of opcodes
    int myStageEntry[NUMSTAGES];       // int array stores the number representing
                                       // each instruction; each index corresponds 
                                       // to a stage; stores -1 if that stage is
                                       // not processing any instruction
    int myStageEmpty;                  // will be set to -1, indicates the a stage is unoccupied 
   
};

#endif
