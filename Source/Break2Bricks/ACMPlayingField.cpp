

#include "Break2Bricks.h"
#include "ACMPlayingField.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Break2BricksPawn.h"
#include "EngineUtils.h"

static const FName g_ssGame("Game");

ACMPlayingField::ACMPlayingField(ABreak2BricksPawn *owner) : ACMachine("ACMPlayingField")
{
	pOwnerActor = owner;

    REGISTER_ACSTATE(ACMPlayingField, Game);
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

void ACMPlayingField::Clicked(ABreak2BricksBlock *pBlock, int iXPos, int iYPos)
{
	tBlockSet aBlocks;
	aBlocks.insert(pBlock);
	FindSameNearBlocks(aBlocks, pBlock);
	if (aBlocks.size() > 1)
	{
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
					MoveBlock(aBlocksField[iBlockIndexX][iBlockIndexY], iBlockIndexX - iLeftStep, iBlockIndexY);
				}
			}
		}
	}
	RelocateBlocks();
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
			return ErrorState("ABreak2BricksBlockGrid not found");
		}

		//ABreak2BricksBlockGrid* NewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlockGrid>();

        // Loop to spawn each block
		for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
		{
			aBlocksField.push_back(std::vector<ABreak2BricksBlock*>());
			for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
			{
				// Spawn a block
				ABreak2BricksBlock* pNewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlock>(FVector(0, 0, 0), FRotator(0, 0, 0));
				aBlocksField.back().push_back(pNewBlock);

				int32 iRand = std::rand() % 3;
				pNewBlock->Init(this, iRand, aBlocksField.size()-1, aBlocksField.back().size()-1);
				// Tell the block about its owner
				if (pNewBlock != nullptr)
				{
					pNewBlock->OwningGrid = pGridActor;
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

        return g_ssGame;
    }
    return "";
}

FName ACMPlayingField::TickStateGame(int iTickType)
{
    return "";
}
