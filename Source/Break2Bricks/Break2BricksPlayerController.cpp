// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksPlayerController.h"
#include "Break2BricksBlock.h"

ABreak2BricksPlayerController::ABreak2BricksPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	TouchFirstClickPressedTime = 0;
}

void ABreak2BricksPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindTouch(IE_Pressed, this, &ABreak2BricksPlayerController::Touched);
}

void ABreak2BricksPlayerController::ResetAnyClick()
{
	bAnyClick = false;
}

bool ABreak2BricksPlayerController::IsAnyClick() const
{
	return bAnyClick;
}

void ABreak2BricksPlayerController::ResetDoubleClick()
{
	bDoubleClick = false;
}

bool ABreak2BricksPlayerController::IsDoubleClick() const
{
	return bDoubleClick;
}

void ABreak2BricksPlayerController::Touched(ETouchIndex::Type FingerIndex, FVector Location)
{
	ClickPressed(FVector(Location.X, Location.Y, 0));
}

bool ABreak2BricksPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	float fLocationX, fLocationY;
	GetMousePosition(fLocationX, fLocationY);

	if ("LeftMouseButton" == Key.GetFName() && EInputEvent::IE_Pressed == EventType)
	{
		ClickPressed(FVector(fLocationX, fLocationY, 0));
	}

	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}

bool IsNeighborBlock(ABreak2BricksBlock *pBlock1, ABreak2BricksBlock *pBlock2, bool &bOutIsDiagonal)
{
	bOutIsDiagonal = false;
	bool bResult = false;
	if (pBlock1 != nullptr && pBlock2 != nullptr && pBlock1->GetType() <= ABreak2BricksBlock::TileType_LastColorType &&
		pBlock1->GetType() == pBlock2->GetType())
	{
		if (1 == abs(pBlock1->GetXPos() - pBlock2->GetXPos()) && pBlock1->GetYPos() == pBlock2->GetYPos() ||
			pBlock1->GetXPos() == pBlock2->GetXPos() && 1 == abs(pBlock1->GetYPos() - pBlock2->GetYPos()))
		{
			bResult = true;
		}
		else
		{
			check(1 == abs(pBlock1->GetXPos() - pBlock2->GetXPos()) && 1 == abs(pBlock1->GetYPos() - pBlock2->GetYPos()));
			bOutIsDiagonal = true;
		}
	}
	return bResult;
}

void ABreak2BricksPlayerController::ClickPressed(const FVector ClickLocation)
{
	bAnyClick = true;
	bDoubleClick = false;
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - TouchFirstClickPressedTime < 0.3)
	{
		bDoubleClick = true;
		TouchFirstClickPressedTime = 0;
	}
	else
	{
		TouchFirstClickPressedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	}

	const int CLICK_CHECK_DIF = 10;
	FVector2D click1(ClickLocation.X + CLICK_CHECK_DIF, ClickLocation.Y + CLICK_CHECK_DIF);
	FVector2D click2(ClickLocation.X - CLICK_CHECK_DIF, ClickLocation.Y - CLICK_CHECK_DIF);
	FHitResult Hit1;
	GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(click1, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, Hit1);
	FHitResult Hit2;
	GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(click2, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, Hit2);
	if (Hit1.GetActor() != nullptr && Hit2.GetActor() != nullptr && Hit1.GetActor() != Hit2.GetActor())
	{
		ABreak2BricksBlock *pBlock1 = Cast<ABreak2BricksBlock>(Hit1.GetActor());
		ABreak2BricksBlock *pBlock2 = Cast<ABreak2BricksBlock>(Hit2.GetActor());
		bool bIsDiagonal;
		if (IsNeighborBlock(pBlock1, pBlock2, bIsDiagonal))
		{
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, TEXT("Neighbor ClickPressed"));
			pBlock1->HandleClicked();
		}
		if (bIsDiagonal)
		{
			FVector2D click3(ClickLocation.X + CLICK_CHECK_DIF, ClickLocation.Y - CLICK_CHECK_DIF);
			FVector2D click4(ClickLocation.X - CLICK_CHECK_DIF, ClickLocation.Y + CLICK_CHECK_DIF);
			FHitResult Hit3;
			GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(click3, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, Hit3);
			FHitResult Hit4;
			GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(click4, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, Hit4);
			if (Hit3.GetActor() != nullptr && Hit4.GetActor() != nullptr && Hit3.GetActor() != Hit4.GetActor())
			{
				ABreak2BricksBlock *pBlock3 = Cast<ABreak2BricksBlock>(Hit3.GetActor());
				ABreak2BricksBlock *pBlock4 = Cast<ABreak2BricksBlock>(Hit4.GetActor());
				if (IsNeighborBlock(pBlock1, pBlock3, bIsDiagonal) && IsNeighborBlock(pBlock2, pBlock4, bIsDiagonal))
				{
					//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, TEXT("Diagonal ClickPressed"));
					pBlock1->HandleClicked();
				}
			}
		}
	}
}
