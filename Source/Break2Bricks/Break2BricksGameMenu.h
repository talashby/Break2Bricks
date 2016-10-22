// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Break2BricksGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UBreak2BricksGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UBreak2BricksGameMenu();

	void Init();

	/** Is Play button pressed */
	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bPlayStandardGamePressed;
	
	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText sCurrentMana;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bDebugNextDayButtonPressed;

	int32 GetMana() const { return iMana; }
	void AddMana(int32 iManaAdd);

private:
	int32 iMana;
};
