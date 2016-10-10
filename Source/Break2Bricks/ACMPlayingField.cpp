

#include "Break2Bricks.h"
#include "ACMPlayingField.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Break2BricksPawn.h"
#include "EngineUtils.h"
#include <algorithm>

static const FName g_ssGame("Game");
static const FName g_ssAnimDropDown("AnimDropDown");
static const FName g_ssConnectColumns("AnimConnectColumns");

ACMPlayingField::ACMPlayingField(ABreak2BricksPawn *owner) : ACMachine("ACMPlayingField")
{
	pOwnerActor = owner;
	bNoMoreMoves = false;

    REGISTER_ACSTATE(ACMPlayingField, Game);
	REGISTER_ACSTATE(ACMPlayingField, AnimDropDown);
	REGISTER_ACSTATE(ACMPlayingField, AnimConnectColumns);
}


ACMPlayingField::~ACMPlayingField()
{
}

void ACMPlayingField::CheckBlockForFindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock, ABreak2BricksBlock *pBlockNear)
{
	if (pBlockNear && pBlockNear->GetType() == pBlock->GetType())
	{
		std::pair<tBlockSet::iterator, bool> ret = aBlocks.insert(pBlockNear);
		if (ret.second)
		{
			FindSameNearBlocks(aBlocks, pBlockNear);
		}
	}
}

void ACMPlayingField::FindSameNearBlocks(tBlockSet &aBlocks, ABreak2BricksBlock *pBlock)
{
	int32 iXPos = pBlock->GetXPos();
	int32 iYPos = pBlock->GetYPos();
	if (iXPos > 0)
	{
		CheckBlockForFindSameNearBlocks(aBlocks, pBlock, aBlocksField[iXPos - 1][iYPos]);
	}
	if (iXPos < aBlocksField.size()-1)
	{
		CheckBlockForFindSameNearBlocks(aBlocks, pBlock, aBlocksField[iXPos + 1][iYPos]);
	}
	if (iYPos > 0)
	{
		CheckBlockForFindSameNearBlocks(aBlocks, pBlock, aBlocksField[iXPos][iYPos-1]);
	}
	if (iYPos < aBlocksField[iXPos].size() - 1)
	{
		CheckBlockForFindSameNearBlocks(aBlocks, pBlock, aBlocksField[iXPos][iYPos+1]);
	}
}

void ACMPlayingField::MoveBlock(ABreak2BricksBlock *pBlock, int32 iDistinationX, int32 iDistinationY)
{
	check(0 <= iDistinationX);
	check(0 <= iDistinationY);
	check(pGridActor->SizeX > iDistinationX);
	check(pGridActor->SizeY > iDistinationY);
	check(nullptr == aBlocksField[iDistinationX][iDistinationY]);
	if (nullptr == pBlock)
	{
		return;
	}
	int iSteps = pBlock->GetYPos() - iDistinationY;
	if (0 < iSteps)
	{
		pBlock->AnimDropDown(iSteps);
		aAnimDropDownBlocks.push_back(pBlock);
	}
	aBlocksField[pBlock->GetXPos()][pBlock->GetYPos()] = nullptr;
	aBlocksField[iDistinationX][iDistinationY] = pBlock;
	pBlock->SetXPos(iDistinationX);
	pBlock->SetYPos(iDistinationY);
}

void ACMPlayingField::RelocateBlocks()
{
	int32 iColumnsNum = 0;
	// calculate columns number
	for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
	{
		if (nullptr != aBlocksField[iBlockIndexX][0])
		{
			++iColumnsNum;
		}
	}

	for (int32 iBlockIndexX = 0; iBlockIndexX < iColumnsNum; ++iBlockIndexX)
	{
		for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
		{
			const float fYOffset = iBlockIndexX * pGridActor->BlockSpacing - (iColumnsNum / 2.f * pGridActor->BlockSpacing);
			const float fXOffset = iBlockIndexY * pGridActor->BlockSpacing - (pGridActor->SizeY / 2.f * pGridActor->BlockSpacing);
			// Make postion vector, offset from Grid location
			const FVector BlockLocation = FVector(fXOffset, fYOffset, 0.f) + pGridActor->GetActorLocation();
			if (nullptr != aBlocksField[iBlockIndexX][iBlockIndexY])
			{
				check(aBlocksField[iBlockIndexX][iBlockIndexY]->GetXPos() == iBlockIndexX);
				check(aBlocksField[iBlockIndexX][iBlockIndexY]->GetYPos() == iBlockIndexY);
				aBlocksField[iBlockIndexX][iBlockIndexY]->SetActorLocation(BlockLocation);
			}
		}
	}
}

void ACMPlayingField::CheckNoMoreMoves()
{
	bNoMoreMoves = true;
	for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
	{
		for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
		{
			tBlockSet aBlocks;
			ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
			if (nullptr != pBlock)
			{
				aBlocks.insert(pBlock);
				FindSameNearBlocks(aBlocks, pBlock);
				if (1 < aBlocks.size())
				{
					bNoMoreMoves = false;
					break;
				}
			}
		}
		if (!bNoMoreMoves)
		{
			break;
		}
	}
}

void ACMPlayingField::Clicked(ABreak2BricksBlock *pBlock, int iXPos, int iYPos)
{
	if (IsCurrentState(g_ssGame))
	{
		tBlockSet aBlocks;
		aBlocks.insert(pBlock);
		FindSameNearBlocks(aBlocks, pBlock);
		if (aBlocks.size() > 1)
		{
			SetStateExternalSignal(EXTERNAL_SIGNAL_1);
			for (ABreak2BricksBlock *pBlockIter : aBlocks)
			{
				int32 iXPosIter = pBlockIter->GetXPos();
				int32 iYPosIter = pBlockIter->GetYPos();
				check(pBlockIter == aBlocksField[iXPosIter][iYPosIter]);
				aBlocksField[iXPosIter][iYPosIter] = nullptr;
				bool bDestroyed = pBlockIter->Destroy();
				check(bDestroyed);
			}

			// gravitation up to down
			for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
			{
				int32 iDownStep = 0;
				for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
				{
					if (aBlocksField[iBlockIndexX][iBlockIndexY] == nullptr)
					{
						++iDownStep;
					}
					else if (iDownStep > 0)
					{
						MoveBlock(aBlocksField[iBlockIndexX][iBlockIndexY], iBlockIndexX, iBlockIndexY - iDownStep);
					}
				}
			}
		}
		//RelocateBlocks();
		CheckNoMoreMoves();
	}
}

void ACMPlayingField::Tick()
{
}

FName ACMPlayingField::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
		// Spawn a block
		TSubclassOf<ABreak2BricksPawn> ClassToFind;
		TArray<AActor*> FoundActors;
		//UGameplayStatics::GetAllActorsOfClass(pOwnerActor->GetWorld(), ClassToFind, FoundActors);
		//int ttt = FoundActors.Num();
		//check(FoundActors.Num() != 1);

		TActorIterator<ABreak2BricksBlockGrid> ActorItr = TActorIterator<ABreak2BricksBlockGrid>(pOwnerActor->GetWorld());
		if (ActorItr)
		{
			pGridActor = *ActorItr;
		}
		else
		{
			return ErrorState("ABreak2BricksBlockGrid not found"); // ******************************* State Finished ********************************
		}

		//ABreak2BricksBlockGrid* NewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlockGrid>();

		if (0 < aBlocksField.size()) // restart case
		{
			for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
			{
				for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
				{
					ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
					if (nullptr != pBlock)
					{
						pBlock->Destroy();
					}
				}
			}
		}
		aBlocksField.clear();

        // Loop to spawn each block
		for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
		{
			aBlocksField.push_back(std::vector<ABreak2BricksBlock*>());
			for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
			{
				// Spawn a block
				//ABreak2BricksBlock* pNewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlock>(FVector(0, 0, 0), FRotator(0, 0, 0));
				int32 iRand = rand() % 3;
				if (pGridActor->aABPBlocks.size() > iRand)
				{
					ABreak2BricksBlock* pNewBlock = SpawnBP<ABreak2BricksBlock>(pOwnerActor->GetWorld(), pGridActor->aABPBlocks[iRand], FVector(0, 0, 0), FRotator(0, 0, 0));
					aBlocksField.back().push_back(pNewBlock);

					
					pNewBlock->Init(this, pGridActor, iRand, aBlocksField.size() - 1, aBlocksField.back().size() - 1);
				}
				else
				{
					WriteLogMessage(ELogVerbosity::Error, "Can't create block actor.");
				}
			}
		}
		RelocateBlocks();
        /*for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
        {
            const float XOffset = (BlockIndex / Size) * pGridActor->BlockSpacing; // Divide by dimension
            const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

                                                                    // Make postion vector, offset from Grid location
            const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

            // Spawn a block
            ABreak2BricksBlock* NewBlock = GetWorld()->SpawnActor<ABreak2BricksBlock>(BlockLocation, FRotator(0, 0, 0));

            // Tell the block about its owner
            if (NewBlock != nullptr)
            {
                NewBlock->OwningGrid = this;
            }
        }*/

        return g_ssGame; // ******************************* State Finished ********************************
    }
    return "";
}

FName ACMPlayingField::TickStateGame(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (IsStateExternalSignal(EXTERNAL_SIGNAL_1))
		{
			return g_ssAnimDropDown; // ******************************* State Finished ********************************
		}
		if (bNoMoreMoves)
		{
			bNoMoreMoves = false;
			return GetStateStartName(); // ******************************* State Finished ********************************
		}
	}
    return "";
}

FName ACMPlayingField::TickStateAnimDropDown(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		aAnimDropDownBlocks.erase(std::remove_if(aAnimDropDownBlocks.begin(), aAnimDropDownBlocks.end(),
			[](const ABreak2BricksBlock* pBlockIter) { return !pBlockIter->bAnimDropDown; }), aAnimDropDownBlocks.end());

		if (aAnimDropDownBlocks.empty())
		{
			// connect columns
			int32 iLeftStep = 0;
			for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
			{
				if (nullptr == aBlocksField[iBlockIndexX][0])
				{
					++iLeftStep;
				}
				else if (iLeftStep > 0)
				{
					for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
					{
						ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
						if (pBlock)
						{
							MoveBlock(pBlock, iBlockIndexX - iLeftStep, iBlockIndexY);
							pBlock->AnimConnectColumns(iLeftStep);
							aAnimConnectColumns.push_back(pBlock);
						}
					}
				}
			}
			if (aAnimConnectColumns.size())
			{
				return g_ssConnectColumns; // ******************************* State Finished ********************************
			}
			else
			{
				return g_ssGame; // ******************************* State Finished ********************************
			}
		}
	}
	return "";
}

FName ACMPlayingField::TickStateAnimConnectColumns(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		check(aAnimConnectColumns.size());
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		aAnimConnectColumns.erase(std::remove_if(aAnimConnectColumns.begin(), aAnimConnectColumns.end(),
			[](const ABreak2BricksBlock* pBlockIter) { return !pBlockIter->bAnimConnectColumns; }), aAnimConnectColumns.end());
		if (!aAnimConnectColumns.size())
		{
			return g_ssGame; // ******************************* State Finished ********************************
		}
	}
	return "";
}
