// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Break2BricksLevelMenu.generated.h"

/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UBreak2BricksLevelMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UBreak2BricksLevelMenu();
	void Init();
	
	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bBombCheckbox;

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText sBombNum;
	void DecBomb();
	void SetBombNum(int32 iBombNum_);
	int32 GetBombNum() const { return iBombNum; }

	/** Hide or show bomb menu */
	UFUNCTION(Category = "Menu", BlueprintImplementableEvent, BlueprintCallable)
	void SetVisibilityBombMenu(bool bIsVisible);

private:
	int32 iBombNum;
};
