// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameField.h"
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

	static UBreak2BricksGameMenu* GetMenu();
	static void UpdateCups();
	void InitOnce();
	void Init();

	/** Is Play button pressed */
	/*UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bPlayStandardGamePressed;*/

	class UMyButtonLevel* GetPressedButtonLevel();
	class UMyButtonFeature* GetPressedButtonFeature();
	class UMyButtonLevel* GetPressedButtonLevelInfo();
	bool GetButtonWorkshopClicked();

	/** Is Clear Saved Data pressed */
	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bClearSavedData;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AGameField *pGameField;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bButtonPressed;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString sCurrentLanguage;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton *pBtnInfo;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UTextBlock*> aCupsNums;

	void OnButtonLevelClicked(class UMyButtonLevel *pUMyButtonLevel);
	void OnButtonFeatureClicked(class UMyButtonFeature *pUMyButtonFeature);

	UFUNCTION()
	void OnButtonInfoClicked();

	UFUNCTION()
	void OnButtonWorkshopClicked();

	UFUNCTION()
	void OnButtonGetManaClicked();

protected:
	void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

private:
	void UpdateManaProgressBar();
	//int32 iMana;
	class UMyButtonLevel *pUMyButtonLevelPressed;
	class UMyButtonLevel *pUMyButtonLevelInfoPressed;
	class UMyButtonFeature *pUMyButtonFeaturePressed;
	bool bButtonWorkshopClicked;

	UProgressBar *pManaProgressBar;
	UTextBlock *pManaProgressBarText;
};
