// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksPawn.h"
#include "Break2BricksBlock.h"
#include "Break2BricksPlayerController.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "DrawDebugHelpers.h"

#include "ACMGeneral.h"

ABreak2BricksPawn::ABreak2BricksPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABreak2BricksPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UCameraComponent* OurCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
				TraceForBlock(Start, End, true);
			}
		}
		else
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);
			TraceForBlock(Start, End, false);
		}
	}

	if (nullptr == spACMGeneral.Get())
	{
		spACMGeneral = SP_ACMGeneral(new ACMGeneral(this));
	}
    spACMGeneral->TickPublic();
    //GetWorld()->GetFirstPlayerController()->ClientMessage("TEST log Message");
}

void ABreak2BricksPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("OnResetVR", EInputEvent::IE_Pressed, this, &ABreak2BricksPawn::OnResetVR);
	PlayerInputComponent->BindAction("TriggerClick", EInputEvent::IE_Pressed, this, &ABreak2BricksPawn::TriggerClick);
}

void ABreak2BricksPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void ABreak2BricksPawn::ResetAnyClick()
{
	if (ABreak2BricksPlayerController* pPC = Cast<ABreak2BricksPlayerController>(GetController()))
	{
		pPC->bAnyClick = false;
	}
}

bool ABreak2BricksPawn::IsAnyClick() const
{
	if (ABreak2BricksPlayerController* pPC = Cast<ABreak2BricksPlayerController>(GetController()))
	{
		return pPC->bAnyClick;
	}
	return false;
}

void ABreak2BricksPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABreak2BricksPawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}

void ABreak2BricksPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		ABreak2BricksBlock* HitBlock = Cast<ABreak2BricksBlock>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}