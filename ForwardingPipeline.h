// Name: Maxine Xin
#ifndef __FORWARDINGPIPELINE_H__
#define __FORWARDINGPIPELINE_H__

#include "Pipeline.h"


class ForwardingPipeline : public Pipeline {
  public:
    /* default constructor of Forwarding Pipeline
     *
     */
    ForwardingPipeline();

    /* destructor
     */
    ~ForwardingPipeline() {};

  private:
    /* given a Stage index st, returns true if the instruction in the
     * preceding stage / the instruction to be fetched 
     * can be moved into Stage st, returns false if otherwise
     *
     */
    bool canMoveIntoStage(int st);
   
     // stage where operand value is needed in forwarding pipeline
    const Stage MY_STAGE_OPERAND_NEEDED = EXECUTE; 

    // stage where operand value is produced in forwarding pipeline for memory instructions
    const Stage MY_STAGE_OPERAND_PRODUCED_MMR = MEMORY; 
    
    // stage where operand value is produced in forwarding pipeline for non-memory instructions
    const Stage MY_STAGE_OPERAND_PRODUCED_NONMMR = EXECUTE; 

    // in this stage, the destination of jump is produced, 
    // is important because instructions cannot be fetched until 
    // the result from this stage is produced
    const Stage MY_STAGE_JUMP_DEST_PRODUCED = DECODE;  
};

#endif
