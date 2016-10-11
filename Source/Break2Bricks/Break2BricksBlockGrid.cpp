// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ABreak2BricksBlockGrid::ABreak2BricksBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);
	
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
	// Set defaults
	SizeX = 3;
	SizeY = 3;
	BlockSpacing = 300.f;
}


void ABreak2BricksBlockGrid::BeginPlay()
{
	Super::BeginPlay();

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


void ABreak2BricksBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
