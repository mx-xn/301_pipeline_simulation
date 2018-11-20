// Name: Maxine Xin
#ifndef __STALLPIPELINE_H__
#define __STALLPIPELINE_H__

#include "Pipeline.h"

class StallPipeline : public Pipeline {
  public:
    /* default constructor of StallPipeline
     *
     */
    StallPipeline();

   /* destructor
    */
   ~StallPipeline() {}; 

  private: 
    /* given a Stage index st, returns true if the instruction in the
     * preceding stage / the instruction to be fetched 
     * can be moved into Stage st, returns false if otherwise
     *
     */
    bool canMoveIntoStage(int st);
    
    // stage where operand value is produced in stall pipeline
    const Stage MYSTAGEOPERANDPRODUCED = WRITEBACK;
    
    // stage where operand value is needed in stall pipeline
    const Stage MYSTAGEOPERANDNEEDED = DECODE;   

    // in this stage, the destination of jump is produced, 
    // is important because instructions cannot be fetched until 
    // the result from this stage is produced
    const Stage MYSTAGEJUMPDESTPRODUCED = DECODE;  
};

#endif
