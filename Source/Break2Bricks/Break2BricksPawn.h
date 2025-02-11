// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Break2BricksPawn.generated.h"

class ACMGeneral; typedef TSharedPtr<ACMGeneral> SP_ACMGeneral;

UCLASS(config=Game)
class ABreak2BricksPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	void ResetAnyClick();
	bool IsAnyClick() const;

	bool IsDoubleClick() const;

protected:
	void OnResetVR();
	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ABreak2BricksBlock* CurrentBlockFocus;

private:

    SP_ACMGeneral spACMGeneral;
};
