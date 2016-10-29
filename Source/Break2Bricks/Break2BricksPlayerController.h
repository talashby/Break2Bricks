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

	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;

	bool bAnyClick;
};


