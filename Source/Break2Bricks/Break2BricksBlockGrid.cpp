// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Components/TextRenderComponent.h"
#include "Break2BricksGameMenu.h"
#include "Break2BricksLevelMenu.h"
#include "MyModalDialog.h"
#include "MyModalWorkshopDialog.h"
#include "MyHudWidget.h"
#include <ctime>

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABreak2BricksBlockGrid *ABreak2BricksBlockGrid::pABreak2BricksBlockGrid = nullptr;

ABreak2BricksBlockGrid::ABreak2BricksBlockGrid()
{
	pABreak2BricksBlockGrid = this;
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	InitialMaxMana = 45;
	ItemMaxMana = 25;
	ItemMaxManaBig = 100;
	ItemMaxManaGreat = 500;

	{
		pGameMenu = nullptr;
		pGameMenuClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_GameMenu.BP_GameMenu_C'"));
		if (ItemBlueprint.Object)
		{
			pGameMenuClass = ItemBlueprint.Object;
		}
	}

	{
		pLevelMenu = nullptr;
		pLevelMenuClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_LevelMenu.BP_LevelMenu_C'"));
		if (ItemBlueprint.Object)
		{
			pLevelMenuClass = ItemBlueprint.Object;
		}
	}

	{
		pModalDialog = nullptr;
		pModalDialogClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_ModalDialog.BP_ModalDialog_C'"));
		if (ItemBlueprint.Object)
		{
			pModalDialogClass = ItemBlueprint.Object;
		}
	}

	{
		pModalDialogFinishLevel = nullptr;
		pModalDialogFinishLevelClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_ModalDialogFinishLevel.BP_ModalDialogFinishLevel_C'"));
		if (ItemBlueprint.Object)
		{
			pModalDialogFinishLevelClass = ItemBlueprint.Object;
		}
	}

	{
		pModalDialogLevelInfo = nullptr;
		pModalDialogLevelInfoClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_ModalDialogLevelInfo.BP_ModalDialogLevelInfo_C'"));
		if (ItemBlueprint.Object)
		{
			pModalDialogLevelInfoClass = ItemBlueprint.Object;
		}
	}

	{
		pModalDialogWorkshop = nullptr;
		pModalDialogWorkshopClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_Workshop.BP_Workshop_C'"));
		if (ItemBlueprint.Object)
		{
			pModalDialogWorkshopClass = ItemBlueprint.Object;
		}
	}

	{
		pModalDialogUpgrade = nullptr;
		pModalDialogUpgradeClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_UpgradeScreen.BP_UpgradeScreen_C'"));
		if (ItemBlueprint.Object)
		{
			pModalDialogUpgradeClass = ItemBlueprint.Object;
		}
	}

	{
		pHUDWidget = nullptr;
		pHUDWidgetClass = nullptr;
		static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("WidgetBlueprint'/Game/Blueprints/BP_HUD.BP_HUD_C'"));
		if (ItemBlueprint.Object)
		{
			pHUDWidgetClass = ItemBlueprint.Object;
		}
	}
	// Set defaults
	SizeX = 10;
	SizeY = 8;
	BlockSpacing = 270.f;
}


void ABreak2BricksBlockGrid::BeginPlay()
{
	srand(time(NULL));
	Super::BeginPlay();
	if (pGameMenuClass)
	{
		pGameMenu = CreateWidget<UBreak2BricksGameMenu>(GetWorld(), pGameMenuClass);
		pGameMenu->InitOnce();
	}
	if (pLevelMenuClass)
	{
		pLevelMenu = CreateWidget<UBreak2BricksLevelMenu>(GetWorld(), pLevelMenuClass);
		//pLevelMenu->Init();
	}
	if (pModalDialogClass)
	{
		pModalDialog = CreateWidget<UMyModalDialog>(GetWorld(), pModalDialogClass);
	}
	if (pModalDialogFinishLevelClass)
	{
		pModalDialogFinishLevel = CreateWidget<UMyModalDialog>(GetWorld(), pModalDialogFinishLevelClass);
	}
	if (pModalDialogUpgradeClass)
	{
		pModalDialogUpgrade = CreateWidget<UMyModalDialog>(GetWorld(), pModalDialogUpgradeClass);
	}
	if (pModalDialogLevelInfoClass)
	{
		pModalDialogLevelInfo = CreateWidget<UMyModalDialog>(GetWorld(), pModalDialogLevelInfoClass);
	}
	if (pModalDialogWorkshopClass)
	{
		pModalDialogWorkshop = CreateWidget<UMyModalWorkshopDialog>(GetWorld(), pModalDialogWorkshopClass);
	}
	if (pHUDWidgetClass)
	{
		pHUDWidget = CreateWidget<UMyHudWidget>(GetWorld(), pHUDWidgetClass);
		pHUDWidget->Init();
		pHUDWidget->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Hud);
	}
	// Number of blocks
	/*const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ABreak2BricksBlock* NewBlock = GetWorld()->SpawnActor<ABreak2BricksBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
		}
	}*/
}

int32 ABreak2BricksBlockGrid::GetManaPrize(const FWinningPrize *pFWinningPrize)
{
	int32 iNum = pFWinningPrize->Mana;

	int32 iBombNum = pLevelMenu->GetBombNum();
	if (iBombNum > 0 && pFWinningPrize->ManaForSavedBombs.Num() > 0)
	{
		iBombNum = iBombNum - 1;
		iBombNum = FMath::Min(iBombNum, pFWinningPrize->ManaForSavedBombs.Num() - 1);
		iNum += pFWinningPrize->ManaForSavedBombs[iBombNum];
	}
	return iNum;
}

void ABreak2BricksBlockGrid::GetPrize(const FWinningPrize *pFWinningPrize, int32 iIndex, int32 &iNum, UTexture2D *&pTexture)
{
	int32 iPrizeIndex = 0;
	int32 iManaPrize = GetManaPrize(pFWinningPrize);
	if (iManaPrize > 0)
	{
		if (iIndex == iPrizeIndex)
		{
			iNum = iManaPrize;
			pTexture = TileManaBonusTexture;
			return;
		}
		++iPrizeIndex;
	}
}

int ABreak2BricksBlockGrid::GetItemSetting(TSubclassOf<class ABreak2BricksBlock> TileClass)
{
	int32 iCounter = 0;
	for (const FItemsSettings &itr : ItemsSettings)
	{
		if (itr.TileClass == TileClass)
		{
			return iCounter;
		}
		++iCounter;
	}
	return -1;
}

/*
void ABreak2BricksBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	pNoMoreMovesText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}*/

#undef LOCTEXT_NAMESPACE
