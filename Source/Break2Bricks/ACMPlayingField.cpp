

#include "Break2Bricks.h"
#include "ACMPlayingField.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Break2BricksPawn.h"
#include "EngineUtils.h"
#include "Break2BricksLevelMenu.h"
#include "MySaveGame.h"
#include "MyButtonLevel.h"
#include "MyHudWidget.h"
#include <random>
#include "MyModalDialog.h"

#define LOCTEXT_NAMESPACE "PlayingField"

static const FName g_ssGame("Game");
static const FName g_ssAnimDropDown("AnimDropDown");
static const FName g_ssConnectColumns("AnimConnectColumns");
static const FName g_ssNoMoreMoves("NoMoreMoves");
static const FName g_ssLevelComplete("LevelComplete");
static const FName g_ssGameFinished("GameFinished");

ACMPlayingField::ACMPlayingField(ABreak2BricksPawn *owner, const FLevelSettings &oFLevelSettings_, const FWinningPrize &oFWinningPrize_) : ACMachine("ACMPlayingField")
{
	pOwnerActor = owner;
	oFLevelSettings = oFLevelSettings_;
	oFWinningPrize = oFWinningPrize_;
	pLastClickedBlock = nullptr;

    REGISTER_ACSTATE(ACMPlayingField, Game);
	REGISTER_ACSTATE(ACMPlayingField, AnimDropDown);
	REGISTER_ACSTATE(ACMPlayingField, AnimConnectColumns);
	REGISTER_ACSTATE(ACMPlayingField, NoMoreMoves);
	REGISTER_ACSTATE(ACMPlayingField, LevelComplete);
	REGISTER_ACSTATE(ACMPlayingField, GameFinished);
}


ACMPlayingField::~ACMPlayingField()
{
	if (0 < aBlocksField.size())
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

float ACMPlayingField::GetTileYOffset(int32 iYPos)
{
	float fYOffset = (pGridActor->SizeY / 2.f * pGridActor->BlockSpacing) - iYPos * pGridActor->BlockSpacing;
	return fYOffset;
}

void ACMPlayingField::RelocateBlocks(bool bWithAnimation)
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
			const float fXOffset = iBlockIndexX * pGridActor->BlockSpacing - ((iColumnsNum - 1) / 2.f * pGridActor->BlockSpacing);
			const float fYOffset = GetTileYOffset(iBlockIndexY);
			// Make position vector, offset from Grid location
			const FVector vecBlockLocation = FVector(fXOffset, fYOffset, 0.f) + pGridActor->GetActorLocation();
			ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
			if (nullptr != pBlock)
			{
				check(pBlock->GetXPos() == iBlockIndexX);
				check(pBlock->GetYPos() == iBlockIndexY);
				if (bWithAnimation)
				{
					if (pBlock->GetActorLocation() != vecBlockLocation)
					{
						pBlock->AnimConnectColumns(vecBlockLocation);
						aAnimConnectColumns.push_back(pBlock);
					}
				}
				else
				{
					pBlock->SetActorLocation(vecBlockLocation);
				}
			}
		}
	}
}

bool ACMPlayingField::CheckNoMoreMoves()
{
	bool bNoMoreMoves = true;
	for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
	{
		for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
		{
			tBlockSet aBlocks;
			ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
			if (nullptr != pBlock)
			{
				if (pBlock->GetType() > ABreak2BricksBlock::TileType_LastColorType)
				{
					bNoMoreMoves = false;
					break;
				}
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
	return bNoMoreMoves;
}

bool ACMPlayingField::CheckLevelComplete()
{
	bool bLevelComplete = true;
	for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
	{
		for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
		{
			tBlockSet aBlocks;
			ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
			if (nullptr != pBlock)
			{
				bLevelComplete = false;
				break;
			}
		}
		if (!bLevelComplete)
		{
			break;
		}
	}
	return bLevelComplete;
}

void ACMPlayingField::DestroyTile(int32 iXPos, int32 iYPos)
{
	int32 iType = aBlocksField[iXPos][iYPos]->GetType();
	check(aTilesNum.size() > iType);
	--aTilesNum[iType];
	check(aTilesNum[iType] >= 0);
	bool bDestroyed = aBlocksField[iXPos][iYPos]->Destroy();
	aBlocksField[iXPos][iYPos] = nullptr;
	check(bDestroyed);
}

void ACMPlayingField::DestroyColorTiles(int32 iXPos, int32 iYPos)
{
	int32 iType = aBlocksField[iXPos][iYPos]->GetType();
	for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
	{
		for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
		{
			tBlockSet aBlocks;
			ABreak2BricksBlock *pBlock = aBlocksField[iBlockIndexX][iBlockIndexY];
			if (pBlock && pBlock->GetType() == iType)
			{
				DestroyTile(iBlockIndexX, iBlockIndexY);
			}
		}
	}
}

int32 ACMPlayingField::GetColorTilesNum() const
{
	check(aTilesNum.size() >= ABreak2BricksBlock::TileType_LastColorType);
	int32 iColorTilesNum = 0;
	for (int32 ii = 0; ii < ABreak2BricksBlock::TileType_LastColorType; ++ii)
	{
		check(aTilesNum[ii] >= 0);
		iColorTilesNum += aTilesNum[ii];
	}
	return iColorTilesNum;
}

bool ACMPlayingField::TestRandom(float fChance, int32 iBlockSize, bool bIsLastColorBlock) const
{
/*
	static int32 iEqualHelper = 0;
	++iEqualHelper;*/

	fChance = fChance * iBlockSize;
	if (bIsLastColorBlock)
	{
		fChance = fChance * 10;
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	float fRand = dist(mt);

	if (fChance > fRand)
	{
		return true;
	}
	return false;
}

bool ACMPlayingField::ClickedItem(ABreak2BricksBlock *pBlock)
{
	for (int ii = 0; ii < UMySaveGame::ITEMS_NUM; ++ii)
	{
		if (pBlock->GetClass() == pGridActor->ItemsSettings[ii].TileClass)
		{
			SetStateExternalSignal(EXTERNAL_SIGNAL_1);
			int32 iXPos = pBlock->GetXPos();
			int32 iYPos = pBlock->GetYPos();
			check(pBlock == aBlocksField[iXPos][iYPos]);
			aBlocksField[iXPos][iYPos] = nullptr;
			bool bDestroyed = pBlock->Destroy();
			check(bDestroyed);
			++UMySaveGame::pUMySaveGame->aItems[ii];
			UMyHudWidget::Update();
			UMySaveGame::SaveGame();
			return true;
		}
	}
	return false;
}

void ACMPlayingField::Clicked(ABreak2BricksBlock *pBlock)
{
	ABreak2BricksBlock *pPrevBlock = pLastClickedBlock;
	pLastClickedBlock = pBlock;
	if (IsCurrentState(g_ssGame))
	{
		if (pBlock->GetClass() == pGridActor->TileManaBonus)
		{
			SetStateExternalSignal(EXTERNAL_SIGNAL_1);
			int32 iXPos = pBlock->GetXPos();
			int32 iYPos = pBlock->GetYPos();
			check(pBlock == aBlocksField[iXPos][iYPos]);
			aBlocksField[iXPos][iYPos] = nullptr;
			bool bDestroyed = pBlock->Destroy();
			check(bDestroyed);
			UMyHudWidget::AddMana(1);
			UMySaveGame::SaveGame();
		}
		else if (ClickedItem(pBlock))
		{
		}
		else if (pBlock->GetClass() == pGridActor->ItemsSettings[UMySaveGame::ITEMSManaBottle].TileClass)
		{

		}
		else if (pLevelMenu->bBombCheckbox || // bomb checkbox clicked
			(UMySaveGame::pUMySaveGame->IsAllowedDetonator() && pOwnerActor->IsDoubleClick() && pLevelMenu->GetBombNum() > 0 && pPrevBlock == pBlock)) // block double clicked
		{
			SetStateExternalSignal(EXTERNAL_SIGNAL_1);
			int32 iXPos = pBlock->GetXPos();
			int32 iYPos = pBlock->GetYPos();
			check(pBlock == aBlocksField[iXPos][iYPos]);
			DestroyTile(iXPos, iYPos);
			pLevelMenu->bBombCheckbox = false;
			pLevelMenu->DecBomb();
		}
		else if (pLevelMenu->bColorBombCheckbox) // color bomb checkbox clicked
		{
			SetStateExternalSignal(EXTERNAL_SIGNAL_1);
			int32 iXPos = pBlock->GetXPos();
			int32 iYPos = pBlock->GetYPos();
			check(pBlock == aBlocksField[iXPos][iYPos]);
			DestroyColorTiles(iXPos, iYPos);
			pLevelMenu->bColorBombCheckbox = false;
			pLevelMenu->DecColorBomb();
		}
		else
		{
			tBlockSet aBlocks;
			aBlocks.insert(pBlock);
			FindSameNearBlocks(aBlocks, pBlock);
			if (aBlocks.size() > 1)
			{
				SetStateExternalSignal(EXTERNAL_SIGNAL_1);
				bool bIsLastColorBlock = false;
				if (GetColorTilesNum() == aBlocks.size())
				{
					bIsLastColorBlock = true;
				}
				std::set<int32> aAutoAddTilesColumns;
				for (ABreak2BricksBlock *pBlockIter : aBlocks)
				{
					int32 iXPosIter = pBlockIter->GetXPos();
					int32 iYPosIter = pBlockIter->GetYPos();
					check(pBlockIter == aBlocksField[iXPosIter][iYPosIter]);
					FVector vecLocTmp = aBlocksField[iXPosIter][iYPosIter]->GetActorLocation();
					DestroyTile(iXPosIter, iYPosIter);
					aAutoAddTilesColumns.insert(iXPosIter);
					bool bBonusSpawned = false;
					{
						for (const FLevelItemsChance &itrSorted : oFLevelSettings.LevelItemsChance)
						{
							if (TestRandom(itrSorted.TileChance, aBlocks.size(), bIsLastColorBlock))
							{
								ABreak2BricksBlock* pNewBlock = SpawnBP<ABreak2BricksBlock>(pOwnerActor->GetWorld(), itrSorted.TileClass, vecLocTmp, FRotator(0, 0, 0));
								aBlocksField[iXPosIter][iYPosIter] = pNewBlock;
								pNewBlock->Init(this, pGridActor, ABreak2BricksBlock::TileType_Bonus, iXPosIter, iYPosIter);
								bBonusSpawned = true;
								break;
							}
						}

					}
					if (!bBonusSpawned && oFLevelSettings.ManaBonus && TestRandom(oFLevelSettings.ManaBonusChance, aBlocks.size(), bIsLastColorBlock) )
					{
						ABreak2BricksBlock* pNewBlock = SpawnBP<ABreak2BricksBlock>(pOwnerActor->GetWorld(), pGridActor->TileManaBonus, vecLocTmp, FRotator(0, 0, 0));
						aBlocksField[iXPosIter][iYPosIter] = pNewBlock;
						pNewBlock->Init(this, pGridActor, ABreak2BricksBlock::TileType_Bonus, iXPosIter, iYPosIter);
					}
				}
				if (oFLevelSettings.AutoAddTiles)
				{
					AddBlocksDynamic(aAutoAddTilesColumns);
				}
				if (oFLevelSettings.AutoAddTilesRandomColumn)
				{
					AddBlocksDynamicRandomColumns();
				}
			}
		}
	}
}

bool ACMPlayingField::IsGameFinished() const
{
	return IsCurrentState(g_ssGameFinished);
}

bool ACMPlayingField::IsLevelComplete() const
{
	return IsLastState(g_ssLevelComplete);
}

void ACMPlayingField::Tick()
{
}

ABreak2BricksBlock* ACMPlayingField::SpawnTile(UClass *pUClass, int32 iType, int32 iXPos, int32 iYPos, const FVector &vecPos)
{
	ABreak2BricksBlock* pNewBlock = SpawnBP<ABreak2BricksBlock>(pOwnerActor->GetWorld(), pUClass, vecPos, FRotator(0, 0, 0));
	if (iType <= ABreak2BricksBlock::TileType_LastColorType)
	{
		++aTilesNum[iType];
	}
	pNewBlock->Init(this, pGridActor, iType, iXPos, iYPos);
	return pNewBlock;
}

FName ACMPlayingField::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {/*
		FString sGameDir = FPaths::GameDir();
		FString sCompleteFilePath = sGameDir + "Content/LevelSettings/MainGame.json";
		FString sResult;
		bool bRes = FFileHelper::LoadFileToString(sResult, *sCompleteFilePath);*/
		// Spawn a block
		//TSubclassOf<ABreak2BricksPawn> ClassToFind;
		//TArray<AActor*> FoundActors;
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

		// pLevelMenu
		{
			pLevelMenu = pGridActor->pLevelMenu;
			if (nullptr == pLevelMenu)
			{
				return ErrorState("UBreak2BricksLevelMenu not found"); // ******************************* State Finished ********************************
			}
			pLevelMenu->Init(oFLevelSettings);

			pLevelMenu->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Back);
			/*if (APlayerController* pPC = Cast<APlayerController>(pOwnerActor->GetController()))
			{
			FInputModeGameAndUI Mode;
			Mode.SetWidgetToFocus(pLevelMenu->TakeWidget());
			pPC->SetInputMode(Mode);
			}*/
		}

		//ABreak2BricksBlockGrid* NewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlockGrid>();

		verify(0 == aBlocksField.size()); // never restart

		aTilesNum.clear();
		aTilesNum.resize(ABreak2BricksBlock::TileType_LastColorType+1, 0);
        // Loop to spawn each block
		for (int32 iBlockIndexX = 0; iBlockIndexX < pGridActor->SizeX; ++iBlockIndexX)
		{
			aBlocksField.push_back(std::vector<ABreak2BricksBlock*>());
			for (int32 iBlockIndexY = 0; iBlockIndexY < pGridActor->SizeY; ++iBlockIndexY)
			{
				// Spawn a block
				//ABreak2BricksBlock* pNewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlock>(FVector(0, 0, 0), FRotator(0, 0, 0));
				int32 iRand = rand() % oFLevelSettings.TileTypesNum;
				if (pGridActor->Tiles.Num() > iRand)
				{
					ABreak2BricksBlock* pNewBlock = SpawnTile(pGridActor->Tiles[iRand], iRand, aBlocksField.size() - 1, aBlocksField.back().size());
					aBlocksField.back().push_back(pNewBlock);
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
		if (CheckLevelComplete())
		{
			return g_ssLevelComplete; // ******************************* State Finished ********************************
		}
		if (CheckNoMoreMoves())
		{
			if (0 < pLevelMenu->GetBombNum())
			{
				if (0 < UMySaveGame::pUMySaveGame->aItems[UMySaveGame::ITEMSBombAutoburn])
				{
					pLevelMenu->BurnBomb();
				}
			}
			else if (0 < pLevelMenu->GetColorBombNum())
			{
				pLevelMenu->BurnColorBomb();
			}
			else
			{
				return g_ssNoMoreMoves; // ******************************* State Finished ********************************
			}
		}
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (IsStateExternalSignal(EXTERNAL_SIGNAL_1))
		{
			return g_ssAnimDropDown; // ******************************* State Finished ********************************
		}
	}
    return "";
}

FName ACMPlayingField::TickStateAnimDropDown(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
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
					ABreak2BricksBlock *pBlock_ = aBlocksField[iBlockIndexX][iBlockIndexY];
					MoveBlock(pBlock_, iBlockIndexX, iBlockIndexY - iDownStep);
					pBlock_->AnimDropDown(iDownStep);
					aAnimDropDownBlocks.push_back(pBlock_);
				}
			}
		}
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		aAnimDropDownBlocks.erase(std::remove_if(aAnimDropDownBlocks.begin(), aAnimDropDownBlocks.end(),
			[](const ABreak2BricksBlock* pBlockIter) { return !pBlockIter->bAnimDropDown; }), aAnimDropDownBlocks.end());

		if (aAnimDropDownBlocks.empty())
		{
			// connect columns
			//RelocateBlocks(true);
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
							//pBlock->AnimConnectColumns(iLeftStep);
							//aAnimConnectColumns.push_back(pBlock);
						}
					}
				}
			}
			RelocateBlocks(true);
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
			RelocateBlocks();
			return g_ssGame; // ******************************* State Finished ********************************
		}
	}
	return "";
}

FName ACMPlayingField::TickStateNoMoreMoves(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		pOwnerActor->ResetAnyClick();
		pLevelMenu->RemoveFromViewport();
		ShowNoMoreMoves();
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pOwnerActor->IsAnyClick())
		{
			pGridActor->pModalDialogFinishLevel->RemoveFromViewport();
			return g_ssGameFinished;
		}
	}
	return "";
}

FName ACMPlayingField::TickStateLevelComplete(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		pOwnerActor->ResetAnyClick();
		//pGridActor->GetLevelCompleteText()->SetVisibility(true);
		ShowLevelComplete();
		pLevelMenu->RemoveFromViewport();

		++UMySaveGame::pUMySaveGame->LevelsPassed;
		UMySaveGame::SaveGame();
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(UMySaveGame::pUMySaveGame->LevelsPassed));
		}*/
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pOwnerActor->IsAnyClick())
		{
			pGridActor->pModalDialogFinishLevel->RemoveFromViewport();
			//pGridActor->GetLevelCompleteText()->SetVisibility(false);
			return g_ssGameFinished;
		}
	}
	return "";
}

FName ACMPlayingField::TickStateGameFinished(int iTickType)
{
	return "";
}

void ACMPlayingField::ShowNoMoreMoves() const
{
	if (pGridActor->pModalDialogFinishLevel)
	{
		pGridActor->pModalDialogFinishLevel->InitFinishLevel(LOCTEXT("No more moves!", "No more moves!"));
		pGridActor->pModalDialogFinishLevel->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
	}
}

void ACMPlayingField::ShowLevelComplete() const
{
	if (pGridActor->pModalDialogFinishLevel)
	{
		pGridActor->pModalDialogFinishLevel->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
		pGridActor->pModalDialogFinishLevel->InitFinishLevel(LOCTEXT("Level completed!", "Level completed!"), &oFWinningPrize);
	}
}

void ACMPlayingField::AddBlocksDynamic(std::set<int32> &aColumns)
{
	for (const int32 &iColumn : aColumns)
	{
		check(iColumn < aBlocksField.size());
		if (aBlocksField[iColumn][pGridActor->SizeY-1] != nullptr)
		{
			continue;
		}
		std::vector<int32> aColors;
		const ABreak2BricksBlock *pTileExample = nullptr;
		for (const ABreak2BricksBlock *pBlockVector : aBlocksField[iColumn])
		{
			if (pBlockVector != nullptr && pBlockVector->GetType() <= ABreak2BricksBlock::TileType_LastColorType)
			{
				aColors.push_back(pBlockVector->GetType());
				pTileExample = pBlockVector;
			}
		}
		if (aColors.size())
		{
			int32 iRnd = MT19937::get(aColors.size()-1);
			int32 iXPosIter = iColumn;
			int32 iYPosIter = pGridActor->SizeY - 1;
			FVector vecLocTmp = FVector(pTileExample->GetActorLocation().X, GetTileYOffset(iYPosIter), 0);
			ABreak2BricksBlock* pNewBlock = SpawnTile(pGridActor->Tiles[aColors[iRnd]], aColors[iRnd], iXPosIter, iYPosIter, vecLocTmp);
			aBlocksField[iXPosIter][iYPosIter] = pNewBlock;
		}
	}
}

int32 ACMPlayingField::GetRandomColorPresentedOnField(int32 iColorTilesNum) const
{
	int32 iRandBlock = rand() % iColorTilesNum;
	{
		int32 iLocalColorTilesNum = 0;
		for (int32 ii = 0; ii < ABreak2BricksBlock::TileType_LastColorType; ++ii)
		{
			check(aTilesNum[ii] >= 0);
			iLocalColorTilesNum += aTilesNum[ii];
			if (iRandBlock < iLocalColorTilesNum)
			{
				return ii;
			}
		}
	}
	check(false);
	return 0;
}

bool ACMPlayingField::SpawnTileOnRandomColumn(int32 iColor)
{
	int32 iColumn = rand() % aBlocksField.size();

	if (aBlocksField[iColumn][pGridActor->SizeY - 1] == nullptr)
	{
		const ABreak2BricksBlock *pTileExample = nullptr;
		for (const ABreak2BricksBlock *pBlockVector : aBlocksField[iColumn])
		{
			if (pBlockVector != nullptr && pBlockVector->GetType() <= ABreak2BricksBlock::TileType_LastColorType)
			{
				pTileExample = pBlockVector;
				break;
			}
		}
		if (pTileExample != nullptr)
		{
			int32 iXPosIter = iColumn;
			int32 iYPosIter = pGridActor->SizeY - 1;
			FVector vecLocTmp = FVector(pTileExample->GetActorLocation().X, GetTileYOffset(iYPosIter), 0);
			ABreak2BricksBlock* pNewBlock = SpawnTile(pGridActor->Tiles[iColor], iColor, iXPosIter, iYPosIter, vecLocTmp);
			aBlocksField[iXPosIter][iYPosIter] = pNewBlock;
			return true;
		}
	}
	return false;
}

void ACMPlayingField::AddBlocksDynamicRandomColumns()
{
	int32 iColorTilesNum = GetColorTilesNum();
	if (iColorTilesNum == 0)
	{
		return;
	}
	int32 iRandColor1 = GetRandomColorPresentedOnField(iColorTilesNum);
	int32 iRandColor2 = GetRandomColorPresentedOnField(iColorTilesNum);

	SpawnTileOnRandomColumn(iRandColor1);
	SpawnTileOnRandomColumn(iRandColor2);
}
#undef  LOCTEXT_NAMESPACE
