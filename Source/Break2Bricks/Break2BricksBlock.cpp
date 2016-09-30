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
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
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
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ABreak2BricksBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ABreak2BricksBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

void ABreak2BricksBlock::Init(ACMPlayingField *pOwnerACM_, int32 iType_, int32 iX, int32 iY)
{
	pOwnerACM = pOwnerACM_;
	iType = iType_;
	iXPos = iX;
	iYPos = iY;
	switch (iType_)
	{
	case 0:
		BlockMesh->SetMaterial(0, BaseMaterial);
		break;
	case 1:
		BlockMesh->SetMaterial(0, BlueMaterial);
		break;
	case 2:
		BlockMesh->SetMaterial(0, OrangeMaterial);
		break;
	default:
		break;
	}
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
	pOwnerACM->Clicked(this, iXPos, iYPos);
	// Change material
	/*BlockMesh->SetMaterial(0, OrangeMaterial);

	// Tell the Grid
	if (OwningGrid != nullptr)
	{
		OwningGrid->AddScore();
	}
	*/
}

void ABreak2BricksBlock::Highlight(bool bOn)
{
	/*
	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}*/
}
