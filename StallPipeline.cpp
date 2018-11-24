// Name: Maxine Xin
#include "StallPipeline.h"

/* default constructor of the StallPipeline
 * 
 */
StallPipeline::StallPipeline() : Pipeline() {
  myName = "STALL";
}

/* given a Stage index st, returns true if the instruction in the
 * preceding stage / the instruction to be fetched
 * can be moved into Stage st, returns false if otherwise 
 *
 */
bool StallPipeline::canMoveIntoStage(int st) {
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
  // and stage where operand is produced if both stages are not empty
  for (int influenceSt = st + 1; influenceSt <= MY_STAGE_OPERAND_PRODUCED; influenceSt++) {
    if (myStageEntry[influenceSt] != myStageEmpty && myStageEntry[st - 1] != myStageEmpty) {
      if (myDepChecker.isRAW(myStageEntry[influenceSt], myStageEntry[st - 1])) {
        // if there is an RAW dependency, there is a stall such that the instruction
        // in the previous stage cannot be moved into the current stage so return false
        return false;
      }
    }
  }
  // no relavant RAW, can move
  return true;
}
