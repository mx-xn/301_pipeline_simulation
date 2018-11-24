// Name: Maxine Xin
#include "ForwardingPipeline.h"

/* default constructor of Forwarding Pipeline
 *
 */
ForwardingPipeline::ForwardingPipeline() : Pipeline() {
  myName = "FORWARDING";
}

/* given a Stage index st, returns true if the instruction in the
 * preceding stage / the instruction to be fetched
 * can be moved into Stage st, returns false if otherwise 
 *
 */
bool ForwardingPipeline::canMoveIntoStage(int st) {
  // if stage is currently occupied
  if (myStageEntry[st] != myStageEmpty)
    return false;
  
  // if the stage where operand is needed is not the previous stage of
  // the current stage, the current stage should be able to get instruction
  // from its previous stage, unless st is the first stage and
  // the destination result of a jump instruction
  // has not been produced yet
  if (st != MY_STAGE_OPERAND_NEEDED + 1) {
    if (st == 0) {
      // if in the first stage, check whether there will be any stalls
      // caused by a jump instruction
      for (int stAffectedByJump = st + 1; 
        stAffectedByJump <= MY_STAGE_JUMP_DEST_PRODUCED; stAffectedByJump++) {
        if (myStageEntry[stAffectedByJump] != myStageEmpty) {
          // if myStageEntry[stAffectedByJump] is not empty,
          // get that instruction
          list<Instruction>::iterator it = myInstructions.begin();
          advance(it, myStageEntry[stAffectedByJump]);

          // if that inst is a jump inst, no inst can be fetched,
          // so cannot move into the first stage, return false
          if (myOpcodeTable.isIMMLabel((*it).getOpcode()))
            return false;
        }
      }
    }
    // if not the first stage, should be able to continue processing
    return true;
  }
  
  // if the stage where operand is needed is indeed the previous stage of
  // the current stage, check whether the inst in the previous stage can be moved,
  // by checking whether there are any RAW dependences between the inst in the 
  // previous stage and the inst in the stage between after the current stage and
  // and stage where operand is produced(for both memory and non_memory inst)
  for (int influenceSt = st + 1; influenceSt <= MY_STAGE_OPERAND_PRODUCED_MMR + 1; influenceSt++) {
    // enters for loop: if influenceSt is between after current stage and the stage 
    // where MMR inst is produced, check whether there is an RAW between inst in 
    // influenceSt and stage where operand is needed if both stages are not empy 
    if (myStageEntry[influenceSt] != myStageEmpty && myStageEntry[MY_STAGE_OPERAND_NEEDED] != myStageEmpty) {
      if (myDepChecker.isRAW(myStageEntry[influenceSt], myStageEntry[MY_STAGE_OPERAND_NEEDED])) {
        // if RAW dependences found, and influenceSt is before the stage after a non_MMR inst 
        // is produced, we do not need to check whether the inst in influenceSt is MMR or NON_MMR, 
        // since stage operand produced for MMR(EXECUTIONs) inst is always later than 
        // stage operand produced for NON_MMR(MEMORYs) inst, and thus there is a stall here regardless,
        // thus cannot move and return false
        if (influenceSt <= MY_STAGE_OPERAND_PRODUCED_NONMMR + 1)
          return false;
     
        // when program reaches here, we do need to check whether inst in 
        // if the previous inst is a MMR inst, since in stage between after operand for 
        // NON_MMR inst is produced and after operand for MMR inst is produced, only a MMR inst
        // will cause stall
        list<Instruction>::iterator it = myInstructions.begin();
        advance(it, myStageEntry[influenceSt]);       // get that inst in influenceSt
   
        // if indeed a MMR inst, will have stall, so cannot move and return false 
        if (myOpcodeTable.isIMMMemory((*it).getOpcode()))
          return false;
        
      }
    }
  }
  // no relavant RAW, can move
  return true;
}
