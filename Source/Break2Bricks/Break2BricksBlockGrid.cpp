// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Components/TextRenderComponent.h"
#include "Break2BricksGameMenu.h"
#include "Break2BricksLevelMenu.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABreak2BricksBlockGrid::ABreak2BricksBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	pNoMoreMovesText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NoMoreMovesText"));
	//pNoMoreMovesText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	//pNoMoreMovesText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	//pNoMoreMovesText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	pNoMoreMovesText->SetupAttachment(DummyRoot);
	//pNoMoreMovesText->SetVisibility(false);
	
	// Create static mesh component
	pLevelCompleteText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LevelCompleteText"));
	//pLevelCompleteText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	//pLevelCompleteText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	pLevelCompleteText->SetupAttachment(DummyRoot);
	//pLevelCompleteText->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint1(TEXT("Blueprint'/Game/Blueprints/BP_Block1.BP_Block1_C'"));
	if (ItemBlueprint1.Object)
	{
		aABPBlocks.push_back(ItemBlueprint1.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint2(TEXT("Blueprint'/Game/Blueprints/BP_Block2.BP_Block2_C'"));
	if (ItemBlueprint2.Object)
	{
		aABPBlocks.push_back(ItemBlueprint2.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint3(TEXT("Blueprint'/Game/Blueprints/BP_Block3.BP_Block3_C'"));
	if (ItemBlueprint3.Object)
	{
		aABPBlocks.push_back(ItemBlueprint3.Object);
	}

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

	// Set defaults
	SizeX = 3;
	SizeY = 3;
	BlockSpacing = 300.f;
}


void ABreak2BricksBlockGrid::BeginPlay()
{
	Super::BeginPlay();
	if (pGameMenuClass)
	{
		pGameMenu = CreateWidget<UBreak2BricksGameMenu>(GetWorld(), pGameMenuClass);
	}
	if (pLevelMenuClass)
	{
		pLevelMenu = CreateWidget<UBreak2BricksLevelMenu>(GetWorld(), pLevelMenuClass);
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


USceneComponent* ABreak2BricksBlockGrid::GetNoMoreMovesText() const
{
	return pNoMoreMovesText;
}

USceneComponent* ABreak2BricksBlockGrid::GetLevelCompleteText() const
{
	return pLevelCompleteText;
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
