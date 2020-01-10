// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MySaveGame.h"
#include "Break2BricksBlockGrid.h"

UMySaveGame *UMySaveGame::pUMySaveGame = nullptr;

void UMySaveGame::OpenButton(int iLevelIndex, const FString &sButtonName)
{
	while (GameMenuSettings.Num() <= iLevelIndex)
	{
		GameMenuSettings.Push(FGameMenuSettings());
	}
	if (!GameMenuSettings[iLevelIndex].FindName(sButtonName))
	{
		GameMenuSettings[iLevelIndex].ButtonNames.Push(sButtonName);
	}
}

bool UMySaveGame::IsAllowedDetonator() const
{
	if (aItems[ITEMSRemoteDetonator] > 0)
		return true;
	return false;
}

void UMySaveGame::SetMainMenuScrollOffset(float fMainMenuScrollOffset_)
{
	if (fMainMenuScrollOffset != fMainMenuScrollOffset_)
	{
		fMainMenuScrollOffset = fMainMenuScrollOffset_;
		SaveGame();
	}
}

int32 UMySaveGame::GetMaxMana()
{
	int32 iMaxMana = ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->InitialMaxMana;
	if (aItems[ITEMSManaBottle] > 0)
	{
		iMaxMana += ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->ItemMaxMana;
	}
	if (aItems[ITEMSBigManaBottle] > 0)
	{
		iMaxMana += ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->ItemMaxManaBig;
	}
	if (aItems[ITEMSGreatManaBottle] > 0)
	{
		iMaxMana += ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->ItemMaxManaGreat;
	}
	if (bDebugMaxMana)
	{
		iMaxMana += 9999;
	}
	iMaxMana += ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.MaxMana1;
	return iMaxMana;
}

UMySaveGame::UMySaveGame()
{
	SaveSlotName = TEXT("SaveSlot0");
	UserIndex = 0;
	FirstStart = true;
	aItems.Init(0, ITEMS_NUM);
	iWorkshopSelectedBox = 0;
	iManaSourceTime = 0;
	DifficultLevelWarning = true;
	fMainMenuScrollOffset = 0;
}

UMySaveGame* UMySaveGame::LoadGame()
{
	UMySaveGame *LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	UMySaveGame *LoadGameInstanceTmp = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	if (LoadGameInstanceTmp != nullptr)
	{
		LoadGameInstance = LoadGameInstanceTmp;
	}
	pUMySaveGame = LoadGameInstance;
	while (pUMySaveGame->aItems.Num() < ITEMS_NUM)
	{
		pUMySaveGame->aItems.Add(0);
	}
	return LoadGameInstance;
}

void UMySaveGame::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(pUMySaveGame, pUMySaveGame->SaveSlotName, pUMySaveGame->UserIndex);
}

