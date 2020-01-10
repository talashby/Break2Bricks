// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyButtonFeatureBase.h"
#include "MyButtonFeature.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "B2B Feature Button"))
class BREAK2BRICKS_API UMyButtonFeature : public UMyButtonFeatureBase
{
	GENERATED_BODY()
public:
	UMyButtonFeature();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	void Update();

	UFUNCTION()
	void OnClick();

	/** Global Settings */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	FGlobalSettings GlobalSettings;

	class UMyButtonLevel *pUMyButtonLevel;
protected:
	
private:
};
