#pragma once

#include "StateMachine/ACMachine.h"
class ABreak2BricksPawn;
class ABreak2BricksBlock;

typedef std::vector<ABreak2BricksBlock*> tBlockVector;
typedef std::set<ABreak2BricksBlock*> tBlockSet;

class ACMPlayingField : public ACMachine
{
public:
    ACMPlayingField(ABreak2BricksPawn *owner);
    virtual ~ACMPlayingField();

	void Clicked(ABreak2BricksBlock *pBlock, int iXPos, int iYPos);

protected:

	void Tick() override;
    virtual FName TickStateStart(int iTickType) override;
    FName TickStateGame(int iTickType);

private:
	void CheckBlockForFindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock, ABreak2BricksBlock *pBlockNear);
	void FindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock);
	void MoveBlock(ABreak2BricksBlock *pBlock, int32 iDistinationX, int32 iDistinationY);
	void RelocateBlocks();
	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;

	std::vector< tBlockVector > aBlocksField;
};

