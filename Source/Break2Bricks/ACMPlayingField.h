#pragma once

#include "StateMachine/ACMachine.h"
#include "LevelSettings.h"

class ABreak2BricksPawn;
class ABreak2BricksBlock;

typedef std::vector<ABreak2BricksBlock*> tBlockVector;
typedef std::set<ABreak2BricksBlock*> tBlockSet;

class ACMPlayingField : public ACMachine
{
public:
    ACMPlayingField(ABreak2BricksPawn *owner, const FLevelSettings &oFLevelSettings_, const FWinningPrize &oFWinningPrize);
    virtual ~ACMPlayingField();

	void Clicked(ABreak2BricksBlock *pBlock);
	bool IsGameFinished() const;
	bool IsLevelComplete() const;

protected:

	void Tick() override;
	virtual FName TickStateStart(int iTickType) override;
    FName TickStateGame(int iTickType);
	FName TickStateAnimDropDown(int iTickType);
	FName TickStateAnimConnectColumns(int iTickType);
	FName TickStateNoMoreMoves(int iTickType);
	FName TickStateLevelComplete(int iTickType);
	FName TickStateGameFinished(int iTickType);

private:
	ABreak2BricksBlock* SpawnTile(UClass *pUClass, int32 iType, int32 iXPos, int32 iYPos, const FVector &vecPos = FVector(0, 0, 0));
	void CheckBlockForFindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock, ABreak2BricksBlock *pBlockNear);
	void FindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock);
	void MoveBlock(ABreak2BricksBlock *pBlock, int32 iDistinationX, int32 iDistinationY);
	float GetTileYOffset(int32 iYPos);
	void RelocateBlocks(bool bWithAnimation = false);
	bool CheckNoMoreMoves();
	bool CheckLevelComplete();
	void DestroyTile(int32 iXPos, int32 iYPos);
	void DestroyColorTiles(int32 iXPos, int32 iYPos);
	int32 GetColorTilesNum() const;
	bool TestRandom(float fChance, int32 iBlockSize, bool bIsLastColorBlock) const;
	bool ClickedItem(ABreak2BricksBlock *pBlock);
	void ShowNoMoreMoves() const;
	void ShowLevelComplete() const;
	void AddBlocksDynamic(std::set<int32> &aColumns);

//*** start
	int32 GetRandomColorPresentedOnField(int32 iColorTilesNum) const;
	bool SpawnTileOnRandomColumn(int32 iColor);
	void AddBlocksDynamicRandomColumns();
// end ***

	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;
	class UBreak2BricksLevelMenu* pLevelMenu;

	std::vector< tBlockVector > aBlocksField;
	tBlockVector aAnimDropDownBlocks;
	tBlockVector aAnimConnectColumns;
	std::vector< int32 > aTilesNum;

	FLevelSettings oFLevelSettings;
	FWinningPrize oFWinningPrize;

	ABreak2BricksBlock *pLastClickedBlock;
};

