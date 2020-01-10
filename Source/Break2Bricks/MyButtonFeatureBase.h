// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "LevelSettings.h"
#include "MyButtonFeatureBase.generated.h"

USTRUCT(BlueprintType)
struct FButtonInfo
{
	GENERATED_BODY()

	// Modal Dialog data
	UPROPERTY(Category = InfoDialog, EditAnywhere, BlueprintReadOnly)
	UTexture2D *InfoImage;

	/** Dialog text */
	UPROPERTY(Category = InfoDialog, EditAnywhere, BlueprintReadOnly)
	FText InfoFText;
};
/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UMyButtonFeatureBase : public UButton
{
	GENERATED_BODY()
	
public:
	UMyButtonFeatureBase();

	/** Level index, 0 - first level */
	int32 LevelIndex;

	/** Is UMyButtonFeature opened or closed */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	bool IsOpen;

	/** Cost for open */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	int32 OpenCost;

	/** Cost for open */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	int32 OpenCostCups;

	UPROPERTY(Category = InfoDialog, EditAnywhere, BlueprintReadOnly)
	TArray<FButtonInfo> ButtonInfoData;

	/** Level Settings */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	FLevelSettings LevelSettings;

	/** Info title */
	FText InfoTitleText;
};
