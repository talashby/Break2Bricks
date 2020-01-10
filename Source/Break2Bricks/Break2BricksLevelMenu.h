// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "LevelSettings.h"

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
	void Init(const FLevelSettings &oFLevelSettings_);
	
	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bRestartButtonPressed;

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bBackButtonPressed;

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bBombCheckbox;

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bColorBombCheckbox;

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText sBombNum;
	void DecBomb();
	void SetBombNum(int32 iBombNum_);
	int32 GetBombNum() const { return iBombNum; }
	UFUNCTION(BlueprintCallable, Category = "BlueprintCallable")
	void BurnBomb();

	UPROPERTY(Category = LevelMenu, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText sColorBombNum;
	void SetColorBombNum(int32 iBombNum_);
	int32 GetColorBombNum() const { return iColorBombNum; }
	UFUNCTION(BlueprintCallable, Category = "BlueprintCallable")
	void BurnColorBomb();
	void DecColorBomb();

	/** Hide or show bomb menu */
	UFUNCTION(Category = "Menu", BlueprintImplementableEvent, BlueprintCallable)
	void SetVisibilityBombMenu(bool bIsVisible);

	/** Hide or show color bomb menu */
	UFUNCTION(Category = "Menu", BlueprintImplementableEvent, BlueprintCallable)
	void SetVisibilityColorBombMenu(bool bIsVisible);

private:
	int32 iBombNum;
	int32 iColorBombNum;
	FLevelSettings oFLevelSettings;
};
