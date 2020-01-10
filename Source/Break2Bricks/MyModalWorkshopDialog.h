// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyModalDialog.h"
#include "MyModalWorkshopDialog.generated.h"

/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UMyModalWorkshopDialog : public UMyModalDialog
{
	GENERATED_BODY()

public:

	UMyModalWorkshopDialog(const FObjectInitializer& ObjectInitializer);

	void InitWorkshop();

	bool GetUpgradeBtnPressed();
	
	int GetSelectedItem() const;
	bool IsBoxSelected() const;

	void UpdateSellBox();
	void UpdateSellBoxVisibility();
	void UpdateUpgradeBox();

	UFUNCTION()
	void OnHiddenButtonClickedBit0();
	UFUNCTION()
	void OnHiddenButtonClickedBit1();
	UFUNCTION()
	void OnHiddenButtonClickedBit2();
	UFUNCTION()
	void OnHiddenButtonClickedBit3();
	UFUNCTION()
	void OnHiddenButtonClickedBit4();
	UFUNCTION()
	void OnHiddenButtonClickedBit5();
	UFUNCTION()
	void OnHiddenButtonClickedBit6();
	UFUNCTION()
	void OnHiddenButtonClickedBit7();

	UFUNCTION()
	void OnHiddenButtonClicked();

	UFUNCTION()
	void OnButtonSellClicked();

	//UFUNCTION()
	void OnButtonUpgradeClicked();

private:

	int32 iHiddenButtonPressedNum;
	bool bUpgradeBtnPressed;
	std::vector<int32> aBoxItems;
};
