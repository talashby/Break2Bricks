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

	/** Is drop down animation played */
	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bPlayStandardGamePressed;
	
};
