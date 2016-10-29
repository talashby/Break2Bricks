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

	void Clicked(ABreak2BricksBlock *pBlock);
	bool IsGameFinished() const;

protected:

	void Tick() override;
    virtual FName TickStateStart(int iTickType) override;
    FName TickStateGame(int iTickType);
	FName TickStateAnimDropDown(int iTickType);
	FName TickStateAnimConnectColumns(int iTickType);
	FName TickStateNoMoreMoves(int iTickType);
	FName TickStateGameFinished(int iTickType);

private:
	void CheckBlockForFindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock, ABreak2BricksBlock *pBlockNear);
	void FindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock);
	void MoveBlock(ABreak2BricksBlock *pBlock, int32 iDistinationX, int32 iDistinationY);
	void RelocateBlocks(bool bWithAnimation=false);
	bool CheckNoMoreMoves();

	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;
	class UBreak2BricksLevelMenu* pLevelMenu;

	std::vector< tBlockVector > aBlocksField;
	tBlockVector aAnimDropDownBlocks;
	tBlockVector aAnimConnectColumns;
};

