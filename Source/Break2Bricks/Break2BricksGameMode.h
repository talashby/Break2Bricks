// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "Break2BricksGameMode.generated.h"

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ABreak2BricksGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABreak2BricksGameMode();
	virtual void InitGameState() override;
	void ClearSavedGame();
	UFUNCTION(exec)
	virtual void B2BAddMana();

protected:
	UPROPERTY()
	class UMySaveGame *LoadGameInstance;
};



