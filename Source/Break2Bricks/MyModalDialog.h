// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MyModalDialog.generated.h"

/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UMyModalDialog : public UUserWidget
{
	GENERATED_BODY()

public:
	UMyModalDialog(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Init(class UMyButtonFeatureBase *pUMyButtonFeatureBase=nullptr);
	void InitFinishLevel(const FText &sTitleText, const struct FWinningPrize *pFWinningPrize = nullptr);
	void InitLevelInfo(class UMyButtonLevel *pUMyButtoLevel, bool bBackButtonVisible=true);
	void InitUpgradeScreen(int32 iSelectedItem);

	static bool GetTextAndImage(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock, UImage *&pOutUImage);
	static bool GetNumberItemsText(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock);

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bBackButtonPressed;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bOKButtonPressed;

	UPROPERTY(Category = GameMenu, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bOKButtonEnabled;

	UFUNCTION()
	void OnButtonUpgradeClicked();
protected:
	UHorizontalBox* GetHorizontalBox(UPanelWidget *pScrollBox, int32 iHorizontalBoxNum);
	bool GetTextImageText(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock, UImage *&pOutUImage, UTextBlock *&pOutTextBlock2);
	bool GetHiddenButton(UHorizontalBox *pHorizontalBox, UButton *&pOutUButton);
	UWidget* DeepDuplicateWidget(UWidget *pUWidget);
	void SetItemDefinition(int32 iItemNum, UTextBlock *pTextBlock);

	class ABreak2BricksBlockGrid *pABreak2BricksBlockGrid;

private:

	int32 iModalDialogUpgrade_SelectedItem;
};
