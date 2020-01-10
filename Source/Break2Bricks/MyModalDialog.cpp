// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyModalDialog.h"
#include "MyModalWorkshopDialog.h"
#include "MyButtonFeatureBase.h"
#include "MyButtonLevel.h"
#include "MyHudWidget.h"
#include "Break2BricksBlockGrid.h"
#include "MySaveGame.h"

#define LOCTEXT_NAMESPACE "Menu" 

UMyModalDialog::UMyModalDialog(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UMyModalDialog::NativeConstruct()
{
	Super::NativeConstruct();

	pABreak2BricksBlockGrid = nullptr;
	MyHelper::GetActorFromScene(GetWorld(), "", pABreak2BricksBlockGrid);
	if (!pABreak2BricksBlockGrid)
	{
		UE_LOG(LogGeneral, Error, TEXT("ABreak2BricksBlockGrid not found in scene"));
		return;
	}
}

void UMyModalDialog::Init(UMyButtonFeatureBase *pUMyButtonFeatureBase)
{
	bBackButtonPressed = false;
	bOKButtonPressed = false;

	UTextBlock *pTextTitle = WidgetTree->FindWidget<UTextBlock>(TEXT("TextTitle"));
	if (pTextTitle)
	{
		pTextTitle->SetText(pUMyButtonFeatureBase->InfoTitleText);
	}

	UTextBlock *pText = WidgetTree->FindWidget<UTextBlock>(TEXT("OpenCostTextBlock"));
	if (pText)
	{
		bOKButtonEnabled = false;
		if (pUMyButtonFeatureBase->IsOpen)
		{
			pText->GetParent()->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			pText->GetParent()->SetVisibility(ESlateVisibility::Visible);
			FText sCost = FText::Format(LOCTEXT("Cost", "Cost: {0}"), pUMyButtonFeatureBase->OpenCost);
			pText->SetText(sCost);
			if (pUMyButtonFeatureBase->OpenCost > 0 && UMyHudWidget::GetMana() >= pUMyButtonFeatureBase->OpenCost)
			{
				bOKButtonEnabled = true;
			}
			// Mana or Cups
			UImage *pManaImg = WidgetTree->FindWidget<UImage>(TEXT("OpenCostMana"));
			UImage *pCupsImg = WidgetTree->FindWidget<UImage>(TEXT("OpenCostCups"));
			if (pManaImg && pCupsImg)
			{
				if (pUMyButtonFeatureBase->OpenCost > 0)
				{
					pManaImg->SetVisibility(ESlateVisibility::Visible);
					pCupsImg->SetVisibility(ESlateVisibility::Hidden);
				}
				else
				{
					sCost = FText::Format(LOCTEXT("Cost", "Cost: {0}"), pUMyButtonFeatureBase->OpenCostCups);
					pText->SetText(sCost);
					pManaImg->SetVisibility(ESlateVisibility::Hidden);
					pCupsImg->SetVisibility(ESlateVisibility::Visible);
					if (UMySaveGame::pUMySaveGame->aCups[pUMyButtonFeatureBase->LevelIndex] >= pUMyButtonFeatureBase->OpenCostCups)
					{
						bOKButtonEnabled = true;
					}
				}
			}
		}
	}

	UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
	if (pScrollBox)
	{
		int32 iHorizontalBoxNum = 0;
		while (true)
		{
			UTextBlock *pTextBlock = nullptr;
			UImage *pUImage = nullptr;
			bool bResult = GetTextAndImage(GetHorizontalBox(pScrollBox, iHorizontalBoxNum), pTextBlock, pUImage);
			if (bResult)
			{
				if (pUImage && pTextBlock)
				{
					if (pUMyButtonFeatureBase->ButtonInfoData.Num() > iHorizontalBoxNum)
					{
						pUImage->SetBrushFromTexture(pUMyButtonFeatureBase->ButtonInfoData[iHorizontalBoxNum].InfoImage, true);
						pTextBlock->SetText(pUMyButtonFeatureBase->ButtonInfoData[iHorizontalBoxNum].InfoFText);
						if (nullptr == pUMyButtonFeatureBase->ButtonInfoData[iHorizontalBoxNum].InfoImage)
						{
							pUImage->Brush.ImageSize.X = 0;
							pUImage->Brush.ImageSize.Y = 0;
						}
					}
					else if (pUMyButtonFeatureBase->LevelSettings.LevelItemsChance.Num() > iHorizontalBoxNum - pUMyButtonFeatureBase->ButtonInfoData.Num())
					{
						TSubclassOf<class ABreak2BricksBlock> TileClass = pUMyButtonFeatureBase->LevelSettings.LevelItemsChance[iHorizontalBoxNum - pUMyButtonFeatureBase->ButtonInfoData.Num()].TileClass;
						int32 iItemNum = pABreak2BricksBlockGrid->GetItemSetting(TileClass);
						if (-1 != iItemNum)
						{
							SetItemDefinition(iItemNum, pTextBlock);
							FText sText = FText::Format(LOCTEXT("Chance", "Chance to receive item {0}"), pTextBlock->GetText());
							pTextBlock->SetText(sText);
							pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Texture, true);
						}
						//pABreak2BricksBlockGrid->ItemsSettings
						//pUImage->SetBrushFromTexture(pUMyButtonFeatureBase->ButtonInfoData[iHorizontalBoxNum].InfoImage, true);
					}
					else
					{
						pUImage->SetBrushFromTexture(nullptr, true);
						pUImage->Brush.ImageSize.X = 0;
						pUImage->Brush.ImageSize.Y = 0;
						pTextBlock->SetText(FText::FromString(""));
					}
				}
				++iHorizontalBoxNum;
			}
			else
			{
				break;
			}
		}
	}
}

void UMyModalDialog::InitFinishLevel(const FText &sTitleText, const FWinningPrize *pFWinningPrize)
{

	UTextBlock *pTextTitle = WidgetTree->FindWidget<UTextBlock>(TEXT("TextTitle"));
	if (pTextTitle)
	{
		pTextTitle->SetText(sTitleText);
	}

	UPanelWidget *pBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScaleBoxCollected"));
	if (pFWinningPrize == nullptr)
	{
		pBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		pBox->SetVisibility(ESlateVisibility::Visible);
	}

	UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
	if (pScrollBox)
	{
		int32 iHorizontalBoxNum = 0;
		for (int32 ii = 0; ii < pScrollBox->GetChildrenCount(); ++ii)
		{
			UWidget *pWidgetChild = pScrollBox->GetChildAt(ii);
			UHorizontalBox *pHorizontalBox = Cast<UHorizontalBox>(pWidgetChild);
			if (pHorizontalBox)
			{
				int32 iPrizeQuantity = 0; UTexture2D *pTexture = nullptr;
				if (pFWinningPrize != nullptr)
				{
					pABreak2BricksBlockGrid->GetPrize(pFWinningPrize, iHorizontalBoxNum, iPrizeQuantity, pTexture);
				}
				for (int32 jj = 0; jj < pHorizontalBox->GetChildrenCount(); ++jj)
				{
					UWidget *pWidgetChild2 = pHorizontalBox->GetChildAt(jj);

					UTextBlock *pTextBlock = Cast<UTextBlock>(pWidgetChild2);
					if (pTextBlock)
					{
						if (iPrizeQuantity != 0)
						{
							pTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d x"), iPrizeQuantity)));
						}
						else
						{
							pTextBlock->SetText(FText::FromString(""));
						}
					}

					UImage *pUImage = Cast<UImage>(pWidgetChild2);
					if (pUImage)
					{
						if (pTexture)
						{
							pUImage->SetBrushFromTexture(pTexture, true);
						}
						else
						{
							pUImage->SetBrushFromTexture(nullptr, true);
							pUImage->Brush.ImageSize.X = 0;
							pUImage->Brush.ImageSize.Y = 0;
						}
					}
				}
				++iHorizontalBoxNum;
			}
		}
	}
}

UHorizontalBox* UMyModalDialog::GetHorizontalBox(UPanelWidget *pScrollBox, int32 iHorizontalBoxNum)
{
	int32 iCurHorizontalBoxNum = 0;
	for (int32 ii = 0; ii < pScrollBox->GetChildrenCount(); ++ii)
	{
		UWidget *pWidgetChild = pScrollBox->GetChildAt(ii);
		UHorizontalBox *pHorizontalBox = nullptr;
		UOverlay *pOverlay = Cast<UOverlay>(pWidgetChild);
		if (pOverlay)
		{
			UWidget *pOverlayWidgetChild = nullptr;
			int32 iIndex = 0;
			do {
				pOverlayWidgetChild = pOverlay->GetChildAt(iIndex);
				pHorizontalBox = Cast<UHorizontalBox>(pOverlayWidgetChild);
				if (pHorizontalBox != nullptr)
				{
					break;
				}
				++iIndex;
			} while (pOverlayWidgetChild != nullptr);
		}
		else
		{
			pHorizontalBox = Cast<UHorizontalBox>(pWidgetChild);
		}
		if (pHorizontalBox)
		{
			if (iCurHorizontalBoxNum == iHorizontalBoxNum)
			{
				return pHorizontalBox;
			}
			++iCurHorizontalBoxNum;
		}
	}
	return nullptr;
}

bool UMyModalDialog::GetTextImageText(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock, UImage *&pOutUImage, UTextBlock *&pOutTextBlock2)
{
	pOutTextBlock = nullptr;
	pOutTextBlock2 = nullptr;
	pOutUImage = nullptr;

	int ii = 0;
	for (; ii < pHorizontalBox->GetChildrenCount(); ++ii)
	{
		UWidget *pWidgetChild = pHorizontalBox->GetChildAt(ii);
		pOutTextBlock = Cast<UTextBlock>(pWidgetChild);
		if (pOutTextBlock != nullptr)
		{
			break;
		}
		else
		{
			pWidgetChild = pHorizontalBox->GetChildAt(ii);
			USizeBox *pUSizeBox = Cast<USizeBox>(pWidgetChild);
			if (pUSizeBox)
			{
				pWidgetChild = pUSizeBox->GetChildAt(0);
				pOutTextBlock = Cast<UTextBlock>(pWidgetChild);
				if (pOutTextBlock != nullptr)
				{
					break;
				}
			}
		}
	}
	for (; ii < pHorizontalBox->GetChildrenCount(); ++ii)
	{
		UWidget *pWidgetChild = pHorizontalBox->GetChildAt(ii);
		pOutUImage = Cast<UImage>(pWidgetChild);
		if (pOutUImage != nullptr)
		{
			break;
		}
	}
	for (; ii < pHorizontalBox->GetChildrenCount(); ++ii)
	{
		UWidget *pWidgetChild = pHorizontalBox->GetChildAt(ii);
		pOutTextBlock2 = Cast<UTextBlock>(pWidgetChild);
		if (pOutTextBlock2 != nullptr)
		{
			break;
		}
	}

	return pOutTextBlock && pOutTextBlock2 && pOutUImage;
}

bool UMyModalDialog::GetTextAndImage(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock, UImage *&pOutUImage)
{
	pOutTextBlock = nullptr;
	pOutUImage = nullptr;
	if (pHorizontalBox)
	{
		for (int32 jj = 0; jj < pHorizontalBox->GetChildrenCount(); ++jj)
		{
			UWidget *pWidgetChild2 = pHorizontalBox->GetChildAt(jj);

			UTextBlock *pTextBlock = Cast<UTextBlock>(pWidgetChild2);
			if (pTextBlock)
			{
				pOutTextBlock = pTextBlock;
			}

			UImage *pUImage = Cast<UImage>(pWidgetChild2);
			if (pUImage)
			{
				pOutUImage = pUImage;
			}
		}
		return true;
	}
	return false;
}

bool UMyModalDialog::GetNumberItemsText(UHorizontalBox *pHorizontalBox, UTextBlock *&pOutTextBlock)
{
	pOutTextBlock = nullptr;
	if (pHorizontalBox)
	{
		for (int32 jj = 0; jj < pHorizontalBox->GetChildrenCount(); ++jj)
		{
			UWidget *pWidgetChild2 = pHorizontalBox->GetChildAt(jj);

			USizeBox *pUSizeBox = Cast<USizeBox>(pWidgetChild2);
			if (pUSizeBox)
			{
				UWidget *pWidgetChild3 = pUSizeBox->GetChildAt(0);
				UTextBlock *pTextBlock = Cast<UTextBlock>(pWidgetChild3);
				if (pTextBlock)
				{
					pOutTextBlock = pTextBlock;
					return true;
				}
			}
		}
	}
	return false;
}

bool UMyModalDialog::GetHiddenButton(UHorizontalBox *pHorizontalBox, UButton *&pOutUButton)
{
	pOutUButton = nullptr;
	if (pHorizontalBox)
	{
		UPanelWidget *pUPanelWidget = pHorizontalBox->GetParent();
		UOverlay *pUOverlay = Cast<UOverlay>(pUPanelWidget);
		if (pUOverlay)
		{
			for (int32 jj = 0; jj < pUOverlay->GetChildrenCount(); ++jj)
			{
				UWidget *pWidgetChild2 = pUOverlay->GetChildAt(jj);

				UButton *pUButton = Cast<UButton>(pWidgetChild2);
				if (pUButton)
				{
					pOutUButton = pUButton;
					return true;
				}
			}
		}
	}
	return false;
}

void UMyModalDialog::InitLevelInfo(class UMyButtonLevel *pUMyButtoLevel, bool bBackButtonVisible)
{
	bBackButtonPressed = false;

	UButton *pBackButton = WidgetTree->FindWidget<UButton>(TEXT("BackButton"));
	if (pBackButton)
	{
		if (bBackButtonVisible)
		{
			pBackButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			pBackButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	UTextBlock *pTextTitle = WidgetTree->FindWidget<UTextBlock>(TEXT("TextTitle"));
	if (pTextTitle)
	{
		pTextTitle->SetText(pUMyButtoLevel->InfoTitleText);
	}

	UTextBlock *pLevelInfoText = WidgetTree->FindWidget<UTextBlock>(TEXT("LevelInfoText"));
	if (pLevelInfoText && pUMyButtoLevel->ButtonInfoData.Num() > 0)
	{
		pLevelInfoText->SetText(pUMyButtoLevel->ButtonInfoData[0].InfoFText);
	}

	UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
	if (pScrollBox)
	{
		int32 iHorizontalBoxNum = 0;
		UTextBlock *pTextBlock;
		UImage *pUImage;
		if (pUMyButtoLevel->WinningPrize.Mana > 0)
		{
			bool bResult = GetTextAndImage(GetHorizontalBox(pScrollBox, iHorizontalBoxNum), pTextBlock, pUImage);
			if (bResult)
			{
				pTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d x"), pUMyButtoLevel->WinningPrize.Mana)));
				pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->TileManaBonusTexture, true);
				++iHorizontalBoxNum;
			}
		}
		for (int ii = 0; ii < pUMyButtoLevel->WinningPrize.ManaForSavedBombs.Num(); ++ii)
		{
			bool bResult = GetTextAndImage(GetHorizontalBox(pScrollBox, iHorizontalBoxNum), pTextBlock, pUImage);
			if (bResult)
			{
				pTextBlock->SetText(FText::Format(LOCTEXT("Prize saved bomb", "{0} saved bomb prize: {1} x"), ii+1, pUMyButtoLevel->WinningPrize.ManaForSavedBombs[ii] + pUMyButtoLevel->WinningPrize.Mana));
				pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->TileManaBonusTexture, true);
				++iHorizontalBoxNum;
			}
		}
		while (true)
		{
			bool bResult = GetTextAndImage(GetHorizontalBox(pScrollBox, iHorizontalBoxNum), pTextBlock, pUImage);
			if (bResult)
			{
				pTextBlock->SetText(FText());
				pUImage->SetBrushFromTexture(nullptr, true);
				pUImage->Brush.ImageSize.X = 0;
				pUImage->Brush.ImageSize.Y = 0;
				++iHorizontalBoxNum;
			}
			else
			{
				break;
			}
		}
	}
}

void UMyModalDialog::InitUpgradeScreen(int32 iSelectedItem)
{
	bBackButtonPressed = false;
	iModalDialogUpgrade_SelectedItem = iSelectedItem;

	int32 iUpgradeCost = 1;
	{
		UHorizontalBox *pUHorizontalBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("HorizontalBoxFrom"));
		UTextBlock *pTextBlockNumber;
		UTextBlock *pTextBlock;
		UImage *pUImage;
		bool bResult = GetTextImageText(pUHorizontalBox, pTextBlockNumber, pUImage, pTextBlock);
		if (bResult)
		{
			iUpgradeCost = pABreak2BricksBlockGrid->ItemsSettings[iModalDialogUpgrade_SelectedItem].UpgradeCost;
			pTextBlockNumber->SetText(FText::Format(FText::FromString("{0}x"), iUpgradeCost));
			pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->ItemsSettings[iModalDialogUpgrade_SelectedItem].Texture, true);
			SetItemDefinition(iModalDialogUpgrade_SelectedItem, pTextBlock);
		}
	}
	{
		UHorizontalBox *pUHorizontalBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("HorizontalBoxTo"));
		UTextBlock *pTextBlockNumber;
		UTextBlock *pTextBlock;
		UImage *pUImage;
		bool bResult = GetTextImageText(pUHorizontalBox, pTextBlockNumber, pUImage, pTextBlock);
		if (bResult)
		{
			pTextBlockNumber->SetText(FText::FromString("1x"));
			int32 iItemNum = pABreak2BricksBlockGrid->ItemsSettings[iModalDialogUpgrade_SelectedItem].UpgradeItem;
			pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Texture, true);
			SetItemDefinition(iItemNum, pTextBlock);
		}
	}

	UButton *pButtonUpgrade = WidgetTree->FindWidget<UButton>(TEXT("ButtonUpgrade"));
	if (pButtonUpgrade)
	{
		if (iUpgradeCost <= UMySaveGame::pUMySaveGame->aItems[iModalDialogUpgrade_SelectedItem])
		{
			pButtonUpgrade->SetIsEnabled(true);
			pButtonUpgrade->OnClicked.AddUniqueDynamic(this, &UMyModalDialog::OnButtonUpgradeClicked);
		}
		else
		{
			pButtonUpgrade->SetIsEnabled(false);
		}
	}
}

void UMyModalDialog::OnButtonUpgradeClicked()
{
	int32 iUpgradeCost = pABreak2BricksBlockGrid->ItemsSettings[iModalDialogUpgrade_SelectedItem].UpgradeCost;
	UMySaveGame::pUMySaveGame->aItems[iModalDialogUpgrade_SelectedItem] -= iUpgradeCost;
	if (UMySaveGame::pUMySaveGame->aItems[iModalDialogUpgrade_SelectedItem] < 0)
	{
		check(false);
		UMySaveGame::pUMySaveGame->aItems[iModalDialogUpgrade_SelectedItem] = 0;
	}

	int32 iNewItemNum = pABreak2BricksBlockGrid->ItemsSettings[iModalDialogUpgrade_SelectedItem].UpgradeItem;
	++UMySaveGame::pUMySaveGame->aItems[iNewItemNum];
	UMyHudWidget::Update();
}

UWidget* UMyModalDialog::DeepDuplicateWidget(UWidget *pUWidget)
{
	UWidget *pNewWidget = DuplicateObject<UWidget>(pUWidget, this);
	UPanelWidget *pNewUPanelWidget = Cast<UPanelWidget>(pNewWidget);
	if (pNewUPanelWidget)
	{
		const TArray<UPanelSlot*>& slots = pNewUPanelWidget->GetSlots();
		for (int32 iSlotNum = 0; iSlotNum < slots.Num(); ++iSlotNum)
		{
			slots[iSlotNum]->Content = nullptr;
		}
		pNewUPanelWidget->ClearChildren();
		UPanelWidget *pUPanelWidget = Cast<UPanelWidget>(pUWidget);
		for (int ii = 0; ii < pUPanelWidget->GetChildrenCount(); ++ii)
		{
			UWidget *pChildUWidget = pUPanelWidget->GetChildAt(ii);
			UWidget *pNewChildWidget = DeepDuplicateWidget(pChildUWidget);
			UPanelSlot *pUPanelSlot = pNewUPanelWidget->AddChild(pNewChildWidget);
			UHorizontalBoxSlot *pNewUHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(pUPanelSlot);
			if (pNewUHorizontalBoxSlot)
			{
				UHorizontalBoxSlot *pUHorizontalBoxSlot = Cast<UHorizontalBoxSlot>(pChildUWidget->Slot);
				pNewUHorizontalBoxSlot->SetHorizontalAlignment(pUHorizontalBoxSlot->HorizontalAlignment);
				pNewUHorizontalBoxSlot->SetVerticalAlignment(pUHorizontalBoxSlot->VerticalAlignment);
			}
			USizeBoxSlot *pNewUSizeBoxSlot = Cast<USizeBoxSlot>(pUPanelSlot);
			if (pNewUSizeBoxSlot)
			{
				USizeBoxSlot *pUSizeBoxSlot = Cast<USizeBoxSlot>(pChildUWidget->Slot);
				pNewUSizeBoxSlot->SetHorizontalAlignment(pUSizeBoxSlot->HorizontalAlignment);
				pNewUSizeBoxSlot->SetVerticalAlignment(pUSizeBoxSlot->VerticalAlignment);
			}
			UOverlaySlot *pNewUOverlaySlot = Cast<UOverlaySlot>(pUPanelSlot);
			if (pNewUOverlaySlot)
			{
				UOverlaySlot *pUOverlaySlot = Cast<UOverlaySlot>(pChildUWidget->Slot);
				pNewUOverlaySlot->SetHorizontalAlignment(pUOverlaySlot->HorizontalAlignment);
				pNewUOverlaySlot->SetVerticalAlignment(pUOverlaySlot->VerticalAlignment);
			}
		}
	}

	return pNewWidget;
}


void UMyModalDialog::SetItemDefinition(int32 iItemNum, UTextBlock *pTextBlock)
{
	if (iItemNum == UMySaveGame::ITEMSManaBottle)
	{
		pTextBlock->SetText(FText::Format(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Definition, pABreak2BricksBlockGrid->ItemMaxMana));
	}
	else if (iItemNum == UMySaveGame::ITEMSBigManaBottle)
	{
		pTextBlock->SetText(FText::Format(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Definition, pABreak2BricksBlockGrid->ItemMaxManaBig));
	}
	else if (iItemNum == UMySaveGame::ITEMSGreatManaBottle)
	{
		pTextBlock->SetText(FText::Format(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Definition, pABreak2BricksBlockGrid->ItemMaxManaGreat));
	}
	else
	{
		pTextBlock->SetText(pABreak2BricksBlockGrid->ItemsSettings[iItemNum].Definition);
	}
}

#undef LOCTEXT_NAMESPACE
