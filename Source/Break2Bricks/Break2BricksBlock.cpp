// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksBlock.h"
#include "Break2BricksBlockGrid.h"
#include "ACMPlayingField.h"

ABreak2BricksBlock::ABreak2BricksBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube")) {}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ABreak2BricksBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ABreak2BricksBlock::OnFingerPressedBlock);

	bAnimDropDown = false;
	bAnimConnectColumns = false;
}

void ABreak2BricksBlock::Init(ACMPlayingField *pOwnerACM_, ABreak2BricksBlockGrid *pOwnerGrid_, int32 iType_, int32 iX, int32 iY)
{
	pOwnerACM = pOwnerACM_;
	OwningGrid = pOwnerGrid_;
	iType = iType_;
	iXPos = iX;
	iYPos = iY;
}

void ABreak2BricksBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void ABreak2BricksBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ABreak2BricksBlock::HandleClicked()
{
	pOwnerACM->Clicked(this);
}

void ABreak2BricksBlock::Highlight(bool bOn)
{
}
