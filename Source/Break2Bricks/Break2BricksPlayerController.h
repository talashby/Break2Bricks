// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Break2BricksPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class ABreak2BricksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABreak2BricksPlayerController();
	void ResetAnyClick();
	bool IsAnyClick() const;
	void ResetDoubleClick();
	bool IsDoubleClick() const;

protected:

	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	virtual void SetupInputComponent() override;

private:
	void ClickPressed(const FVector ClickLocation);
	void Touched(ETouchIndex::Type FingerIndex, FVector Location);
	bool bAnyClick;

	bool bDoubleClick;
	float TouchFirstClickPressedTime;
};


