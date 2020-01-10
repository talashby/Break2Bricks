// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyButtonFeatureBase.h"
#include "MyButtonLevel.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "B2B Level Button"))
class BREAK2BRICKS_API UMyButtonLevel : public UMyButtonFeatureBase
{
	GENERATED_BODY()
public:
	UMyButtonLevel();

	UFUNCTION()
	void OnClick();

	void Update();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	/** Cost for one game */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	int32 PlayCost;

	FLevelSettings LevelSettingsCopy;

	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	FWinningPrize WinningPrize;

	/** Difficult level warning */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	bool DifficultLevel;

protected:
};
