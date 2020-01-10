// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyModalWorkshopDialog.h"

#include "MyHudWidget.h"
#include "Break2BricksBlockGrid.h"
#include "MySaveGame.h"

UMyModalWorkshopDialog::UMyModalWorkshopDialog(const FObjectInitializer& ObjectInitializer) : UMyModalDialog(ObjectInitializer)
{

}

void UMyModalWorkshopDialog::InitWorkshop()
{
	bUpgradeBtnPressed = false;
	iHiddenButtonPressedNum = 0;
	bBackButtonPressed = false;
	aBoxItems.clear();

	if (pABreak2BricksBlockGrid->ItemsSettings.Num() != UMySaveGame::ITEMS_NUM)
	{
		UE_LOG(LogGeneral, Error, TEXT("pABreak2BricksBlockGrid->ItemsSettings.Num() is wrong"));
		return;
	}

	UBorder *pUBorder = WidgetTree->FindWidget<UBorder>(TEXT("BorderSelect"));
	if (!pUBorder)
	{
		UE_LOG(LogGeneral, Error, TEXT("BorderSelect widget not found"));
		return;
	}
	pUBorder->GetParent()->RemoveChild(pUBorder);

	int32 iHorizontalBoxNum = 0;
	UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
	if (pScrollBox)
	{
		UTextBlock *pTextBlockNumber;
		UTextBlock *pTextBlock;
		UImage *pUImage;
		UButton *pUButton;
		for (int32 ii = 0; ii < UMySaveGame::ITEMS_NUM; ++ii)
		{
			if (UMySaveGame::pUMySaveGame->aItems[ii] > 0)
			{
				aBoxItems.push_back(ii);
				UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, iHorizontalBoxNum);
				if (!pUHorizontalBox)
				{
					UHorizontalBox *pUPreviousHorizontalBox = GetHorizontalBox(pScrollBox, iHorizontalBoxNum - 1);
					if (pUPreviousHorizontalBox)
					{
						UWidget *pUWidget = DeepDuplicateWidget(pUPreviousHorizontalBox->GetParent());
						//pUHorizontalBox = Cast<UHorizontalBox>(pUWidget);
						pScrollBox->AddChild(pUWidget);
						pUHorizontalBox = GetHorizontalBox(pScrollBox, iHorizontalBoxNum);
						if (!pUHorizontalBox)
						{
							UE_LOG(LogGeneral, Error, TEXT("Deep duplication failed"));
							return;
						}
					}
				}
				pUHorizontalBox->SetVisibility(ESlateVisibility::Visible);
				bool bResult = GetTextAndImage(pUHorizontalBox, pTextBlock, pUImage);
				bResult &= GetNumberItemsText(pUHorizontalBox, pTextBlockNumber);
				bResult &= GetHiddenButton(pUHorizontalBox, pUButton);
				if (bResult)
				{
					pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->ItemsSettings[ii].Texture, true);
					SetItemDefinition(ii, pTextBlock);
					pTextBlockNumber->SetText(FText::Format(FText::FromString("{0}x"), UMySaveGame::pUMySaveGame->aItems[ii]));
					pUButton->SetVisibility(ESlateVisibility::Visible);
					pUButton->OnClicked = FOnButtonClickedEvent();
					if (0x1 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit0);
					}
					if (0x2 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit1);
					}
					if (0x4 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit2);
					}
					if (0x8 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit3);
					}
					if (0x10 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit4);
					}
					if (0x20 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit5);
					}
					if (0x40 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit6);
					}
					if (0x80 & (iHorizontalBoxNum + 1))
					{
						pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClickedBit7);
					}
					pUButton->OnClicked.AddDynamic(this, &UMyModalWorkshopDialog::OnHiddenButtonClicked);
					++iHorizontalBoxNum;
				}
			}
		}
	}
	// hide unused boxes
	for (int iClear = iHorizontalBoxNum; true; ++iClear)
	{
		UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, iClear);
		if (!pUHorizontalBox)
		{
			break;
		}
		pUHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		UButton *pUButton;
		bool bResult = GetHiddenButton(pUHorizontalBox, pUButton);
		if (bResult)
		{
			pUButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	// Border Selector
	if (iHorizontalBoxNum > 0)
	{
		check(UMySaveGame::pUMySaveGame->iWorkshopSelectedBox >= 0);
		UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, UMySaveGame::pUMySaveGame->iWorkshopSelectedBox);
		UPanelSlot *pUPanelSlot = pUHorizontalBox->GetParent()->AddChild(pUBorder);
		pUBorder->SetVisibility(ESlateVisibility::Visible);
		UOverlaySlot *pUOverlaySlot = Cast<UOverlaySlot>(pUPanelSlot);
		if (pUOverlaySlot)
		{
			pUOverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			pUOverlaySlot->SetVerticalAlignment(VAlign_Fill);
		}
	}
	else
	{
		UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, 0);
		UPanelSlot *pUPanelSlot = pUHorizontalBox->GetParent()->AddChild(pUBorder);
		pUBorder->SetVisibility(ESlateVisibility::Hidden);
	}
	UpdateSellBox();
	UpdateSellBoxVisibility();
	UpdateUpgradeBox();

	UButton *pButtonSell = WidgetTree->FindWidget<UButton>(TEXT("ButtonSell"));
	if (pButtonSell)
	{
		pButtonSell->OnClicked.AddUniqueDynamic(this, &UMyModalWorkshopDialog::OnButtonSellClicked);
	}
	UButton *pButtonUpgrade = WidgetTree->FindWidget<UButton>(TEXT("ButtonUpgrade"));
	if (pButtonUpgrade)
	{
		pButtonUpgrade->OnClicked.AddUniqueDynamic(this, &UMyModalWorkshopDialog::OnButtonUpgradeClicked);
	}
}

bool UMyModalWorkshopDialog::GetUpgradeBtnPressed()
{
	bool bTmp = bUpgradeBtnPressed;
	bUpgradeBtnPressed = false;
	return bTmp;
}

int UMyModalWorkshopDialog::GetSelectedItem() const
{
	if (aBoxItems.size() <= UMySaveGame::pUMySaveGame->iWorkshopSelectedBox)
	{
		return 0;
	}
	return aBoxItems[UMySaveGame::pUMySaveGame->iWorkshopSelectedBox];
}

bool UMyModalWorkshopDialog::IsBoxSelected() const
{
	return aBoxItems.size() != 0;
}

void UMyModalWorkshopDialog::UpdateSellBox()
{
	UHorizontalBox *pUHorizontalBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("SellHorizontalBox"));
	if (pUHorizontalBox && IsBoxSelected())
	{
		UTextBlock *pTextBlock;
		UImage *pUImage;
		bool bResult = GetTextAndImage(pUHorizontalBox, pTextBlock, pUImage);
		if (bResult)
		{
			pTextBlock->SetText(FText::Format(FText::FromString("{0}x"), pABreak2BricksBlockGrid->ItemsSettings[GetSelectedItem()].SellManaCost));
			pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->TileManaBonusTexture, true);
		}
	}
}

void UMyModalWorkshopDialog::UpdateSellBoxVisibility()
{
	UHorizontalBox *pUHorizontalBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("SellHorizontalBox"));
	UButton *pButtonSell = WidgetTree->FindWidget<UButton>(TEXT("ButtonSell"));
	if (pButtonSell && pUHorizontalBox)
	{
		int iItemsNum = UMySaveGame::pUMySaveGame->aItems[GetSelectedItem()];
		if (iItemsNum > 0)
		{
			pUHorizontalBox->SetVisibility(ESlateVisibility::Visible);
			pButtonSell->SetIsEnabled(true);
		}
		else
		{
			pUHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
			pButtonSell->SetIsEnabled(false);
		}
	}
}

void UMyModalWorkshopDialog::UpdateUpgradeBox()
{
	UButton *pButtonUpgrade = WidgetTree->FindWidget<UButton>(TEXT("ButtonUpgrade"));
	UHorizontalBox *pUHorizontalBox = WidgetTree->FindWidget<UHorizontalBox>(TEXT("UpgradeHorizontalBox"));
	if (pUHorizontalBox)
	{
		int32 iUpgradeCost = pABreak2BricksBlockGrid->ItemsSettings[GetSelectedItem()].UpgradeCost;
		if (iUpgradeCost > 0 && IsBoxSelected())
		{
			UTextBlock *pTextBlock;
			UImage *pUImage;
			bool bResult = GetTextAndImage(pUHorizontalBox, pTextBlock, pUImage);
			if (bResult)
			{
				pTextBlock->SetText(FText::Format(FText::FromString("{0}x"), iUpgradeCost));
				pUImage->SetBrushFromTexture(pABreak2BricksBlockGrid->ItemsSettings[GetSelectedItem()].Texture, true);
			}
			pUHorizontalBox->SetVisibility(ESlateVisibility::Visible);
			pButtonUpgrade->SetIsEnabled(true);
		}
		else
		{
			pUHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
			pButtonUpgrade->SetIsEnabled(false);
		}
	}
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit0()
{
	iHiddenButtonPressedNum |= 0x1;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit1()
{
	iHiddenButtonPressedNum |= 0x2;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit2()
{
	iHiddenButtonPressedNum |= 0x4;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit3()
{
	iHiddenButtonPressedNum |= 0x8;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit4()
{
	iHiddenButtonPressedNum |= 0x10;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit5()
{
	iHiddenButtonPressedNum |= 0x20;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit6()
{
	iHiddenButtonPressedNum |= 0x40;
}

void UMyModalWorkshopDialog::OnHiddenButtonClickedBit7()
{
	iHiddenButtonPressedNum |= 0x80;
}

void UMyModalWorkshopDialog::OnHiddenButtonClicked()
{
	if (iHiddenButtonPressedNum - 1 != UMySaveGame::pUMySaveGame->iWorkshopSelectedBox)
	{
		UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
		UBorder *pUBorder = WidgetTree->FindWidget<UBorder>(TEXT("BorderSelect"));
		pUBorder->GetParent()->RemoveChild(pUBorder);
		UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, iHiddenButtonPressedNum - 1);
		UPanelSlot *pUPanelSlot = pUHorizontalBox->GetParent()->AddChild(pUBorder);
		UOverlaySlot *pUOverlaySlot = Cast<UOverlaySlot>(pUPanelSlot);
		if (pUOverlaySlot)
		{
			pUOverlaySlot->SetHorizontalAlignment(HAlign_Fill);
			pUOverlaySlot->SetVerticalAlignment(VAlign_Fill);
		}

		UMySaveGame::pUMySaveGame->iWorkshopSelectedBox = iHiddenButtonPressedNum - 1;
		UpdateSellBox();
		UpdateSellBoxVisibility();
		UpdateUpgradeBox();
	}

	iHiddenButtonPressedNum = 0;
}

void UMyModalWorkshopDialog::OnButtonSellClicked()
{
	int &iItemsNum = UMySaveGame::pUMySaveGame->aItems[GetSelectedItem()];
	if (iItemsNum > 0)
	{
		UMyHudWidget::GetMenu()->AddMana(pABreak2BricksBlockGrid->ItemsSettings[GetSelectedItem()].SellManaCost);
		--iItemsNum;
		UPanelWidget *pScrollBox = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBox"));
		UHorizontalBox *pUHorizontalBox = GetHorizontalBox(pScrollBox, UMySaveGame::pUMySaveGame->iWorkshopSelectedBox);
		UTextBlock *pTextBlockNumber;
		bool bResult = GetNumberItemsText(pUHorizontalBox, pTextBlockNumber);
		if (bResult)
		{
			if (iItemsNum == 0)
			{
				pTextBlockNumber->SetText(FText());
				if (UMySaveGame::pUMySaveGame->iWorkshopSelectedBox > 0)
				{
					--UMySaveGame::pUMySaveGame->iWorkshopSelectedBox;
				}
				UMyHudWidget::Update();
				InitWorkshop();
			}
			else
			{
				pTextBlockNumber->SetText(FText::Format(FText::FromString("{0}x"), UMySaveGame::pUMySaveGame->aItems[GetSelectedItem()]));
			}
		}
		UpdateSellBoxVisibility();
	}
}

void UMyModalWorkshopDialog::OnButtonUpgradeClicked()
{
	bUpgradeBtnPressed = true;
}

