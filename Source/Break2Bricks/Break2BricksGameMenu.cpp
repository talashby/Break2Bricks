// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "Break2BricksGameMenu.h"
#include "MySaveGame.h"
#include "MyButtonLevel.h"
#include "MyButtonFeature.h"
#include "Break2BricksBlockGrid.h"
#include "MyHudWidget.h"
#include "MyModalDialog.h"

#define LOCTEXT_NAMESPACE "GameMenu" 

static UBreak2BricksGameMenu *s_pUBreak2BricksGameMenu;

UBreak2BricksGameMenu::UBreak2BricksGameMenu() : UUserWidget(FObjectInitializer())
{
	//sCurrentMana = LOCTEXT("ManaFmt", "Mana: 90");
	//iMana = 0;
	//AddMana(100);
	//sCurrentMana = LOCTEXT("ManaTMP", "Mana: 100");
	//Init();
	s_pUBreak2BricksGameMenu = this;
	pUMyButtonLevelPressed = nullptr;
	pUMyButtonFeaturePressed = nullptr;
	pUMyButtonLevelInfoPressed = nullptr;
	pManaProgressBar = nullptr;
	pManaProgressBarText = nullptr;
}

UBreak2BricksGameMenu* UBreak2BricksGameMenu::GetMenu()
{
	return s_pUBreak2BricksGameMenu;
}

void UBreak2BricksGameMenu::UpdateCups()
{
	for (int ii = 0; ii < GetMenu()->aCupsNums.Num(); ++ii)
	{
		GetMenu()->aCupsNums[ii]->SetText(FText::FromString(FString::FromInt(UMySaveGame::pUMySaveGame->aCups[ii])));
	}
}

void UBreak2BricksGameMenu::InitOnce()
{
	bButtonWorkshopClicked = false;
	aCupsNums.Empty();

	ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings = FGlobalSettings();

	UComboBoxString *pLanguageBox = WidgetTree->FindWidget<UComboBoxString>(TEXT("LanguageBox"));
	if (pLanguageBox)
	{
		sCurrentLanguage = "English";
		if (!UMySaveGame::pUMySaveGame->CurrentLanguage.IsEmpty())
		{
			sCurrentLanguage = UMySaveGame::pUMySaveGame->CurrentLanguage;
		}
		else
		{
			UMySaveGame::pUMySaveGame->CurrentLanguage = sCurrentLanguage;
		}
		pLanguageBox->SetSelectedOption(sCurrentLanguage);

		if (sCurrentLanguage == "English")
		{
			FInternationalization::Get().SetCurrentCulture("en");
		}
		else if (sCurrentLanguage == "Russian")
		{
			FInternationalization::Get().SetCurrentCulture("ru");
		}
	}
	
	{
		UButton *pUButton = WidgetTree->FindWidget<UButton>(TEXT("Button_Workshop"));
		if (pUButton)
		{
			pUButton->OnClicked.AddUniqueDynamic(this, &UBreak2BricksGameMenu::OnButtonWorkshopClicked);
		}
	}

	UScrollBox *pScrollBox = WidgetTree->FindWidget<UScrollBox>(TEXT("ScrollBoxLevels"));
	pScrollBox->SetScrollOffset(UMySaveGame::pUMySaveGame->fMainMenuScrollOffset);
	int32 iLevel = 0;
	UBorder *pLastBorder = nullptr;
	bool bFirstClosed = true;
	bool IsInPreviousRowAllElementsOpen = true;
	for (int32 ii = 0; ii < pScrollBox->GetChildrenCount(); ++ii)
	{
		bool bFirstFeatureBtnClosed = false;
		UWidget *pWidgetChild = pScrollBox->GetChildAt(ii);
		UBorder *pBorder = Cast<UBorder>(pWidgetChild);
		if (pBorder)
		{
			pLastBorder = pBorder;
		}
		UHorizontalBox *pHorizontalBox = Cast<UHorizontalBox>(pWidgetChild);
		if (pHorizontalBox)
		{
			bool IsInThisRowAllElementsOpen = true;
			UMyButtonLevel *pUMyButtonLevel = nullptr;
			for (int32 jj = 0; jj < pHorizontalBox->GetChildrenCount(); ++jj)
			{
				UWidget *pWidgetChild2 = pHorizontalBox->GetChildAt(jj);
				UVerticalBox *pVerticalBox = Cast<UVerticalBox>(pWidgetChild2);
				if (pVerticalBox)
				{
					if (ESlateVisibility::Hidden == pVerticalBox->GetVisibility())
					{
						continue;
					}
					pVerticalBox->SetIsEnabled(true);
					for (int32 kk = 0; kk < pVerticalBox->GetChildrenCount(); ++kk)
					{
						// button level features
						if (!pUMyButtonLevel)
						{
							pUMyButtonLevel = Cast<UMyButtonLevel>(pVerticalBox->GetChildAt(kk));
							if (pUMyButtonLevel)
							{
								pUMyButtonLevel->LevelIndex = iLevel;
								++iLevel;
								if (pUMyButtonLevel->LevelIndex != 0)
								{
									pUMyButtonLevel->IsOpen = false;
								}
								if (UMySaveGame::pUMySaveGame->GameMenuSettings.Num() > pUMyButtonLevel->LevelIndex)
								{
									pUMyButtonLevel->IsOpen = UMySaveGame::pUMySaveGame->GameMenuSettings[pUMyButtonLevel->LevelIndex].FindName(pUMyButtonLevel->GetName());
								}
								if (pUMyButtonLevel->LevelIndex == 0)
								{
									pUMyButtonLevel->IsOpen = true;
								}
								pUMyButtonLevel->LevelSettingsCopy = pUMyButtonLevel->LevelSettings;
								pUMyButtonLevel->Update();
								if (!pUMyButtonLevel->IsOpen)
								{
									if (bFirstClosed)
									{
										bFirstClosed = false;
										if (1 == pUMyButtonLevel->LevelIndex && !IsInPreviousRowAllElementsOpen)
										{
											pHorizontalBox->SetIsEnabled(false);
											if (pLastBorder)
											{
												pLastBorder->SetIsEnabled(false);
											}
										}
										else
										{
											pHorizontalBox->SetIsEnabled(true);
										}
									}
									else
									{
										pHorizontalBox->SetIsEnabled(false);
										if (pLastBorder)
										{
											pLastBorder->SetIsEnabled(false);
										}
									}
								}
								// Button info init
								UWidget *pCostWidget = pHorizontalBox->GetChildAt(1);
								UButton *pBtnInfo_ = Cast<UButton>(pCostWidget);
								if (pBtnInfo_)
								{
									pBtnInfo_->OnClicked.AddUniqueDynamic(this, &UBreak2BricksGameMenu::OnButtonInfoClicked); // Example click binding.
								}
								// Cups init
								if (UMySaveGame::pUMySaveGame->aCups.Num() <= pUMyButtonLevel->LevelIndex)
								{
									UMySaveGame::pUMySaveGame->aCups.Push(0);
								}
								UWidget *pCupsWidget = pHorizontalBox->GetChildAt(0);
								USizeBox *pCupsSB = Cast<USizeBox>(pCupsWidget);
								if (pCupsSB)
								{
									UWidget *pCupsWidget2 = pCupsSB->GetChildAt(0);
									UHorizontalBox *pCupsHB = Cast<UHorizontalBox>(pCupsWidget2);
									if (pCupsHB)
									{
										UTextBlock *pOutTextBlock;
										UImage *pOutUImage;
										UMyModalDialog::GetTextAndImage(pCupsHB, pOutTextBlock, pOutUImage);
										aCupsNums.Push(pOutTextBlock);
									}
								}
							}
						}
						// buttonFeature properties
						else /*!pUMyButtonLevel*/
						{
							UMyButtonFeature *pUMyButtonFeature = Cast<UMyButtonFeature>(pVerticalBox->GetChildAt(kk));
							if (pUMyButtonFeature)
							{
								pUMyButtonFeature->pUMyButtonLevel = pUMyButtonLevel;
								pUMyButtonFeature->LevelIndex = pUMyButtonLevel->LevelIndex;
								FString sName;
								pUMyButtonFeature->GetName(sName);
								pUMyButtonFeature->IsOpen = false;
								if (UMySaveGame::pUMySaveGame->GameMenuSettings.Num() > pUMyButtonLevel->LevelIndex)
								{
									pUMyButtonFeature->IsOpen = UMySaveGame::pUMySaveGame->GameMenuSettings[pUMyButtonLevel->LevelIndex].FindName(sName);
								}
								if (pUMyButtonFeature->IsOpen)
								{
									pUMyButtonLevel->LevelSettingsCopy.Add(pUMyButtonFeature->LevelSettings);
									ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.Add(pUMyButtonFeature->GlobalSettings);
								}
								else
								{
									IsInThisRowAllElementsOpen = false;
									if (!bFirstFeatureBtnClosed)
									{
										bFirstFeatureBtnClosed = true;
										if (!pUMyButtonLevel->IsOpen)
										{
											pVerticalBox->SetIsEnabled(false);
										}
									}
									else
									{
										pVerticalBox->SetIsEnabled(false);
									}
								}
								pUMyButtonFeature->Update();
							}
						}
						// common features
						UMyButtonFeatureBase *pUMyFeatureBase = Cast<UMyButtonFeatureBase>(pVerticalBox->GetChildAt(kk));
						if (pUMyFeatureBase)
						{
							if (pVerticalBox->GetChildrenCount() >= kk + 1)
							{
								UWidget *pCostWidget = pVerticalBox->GetChildAt(kk + 1);
								UTextBlock *pText = Cast<UTextBlock>(pCostWidget);
								UHorizontalBox *pHorizontalBoxCost = nullptr;
								if (!pText)
								{
									pHorizontalBoxCost = Cast<UHorizontalBox>(pCostWidget);
									pCostWidget = pHorizontalBoxCost->GetChildAt(0);
									pText = Cast<UTextBlock>(pCostWidget);
								}
								if (pText)
								{
									if (pUMyFeatureBase->IsOpen)
									{
										pText->SetText(FText());
										if (pHorizontalBoxCost)
										{
											pHorizontalBoxCost->SetVisibility(ESlateVisibility::Hidden);
										}
									}
									else
									{
										pText->SetText(FText::Format(LOCTEXT("Cost", "Cost: {0}"), pUMyFeatureBase->OpenCost));
										if (pHorizontalBoxCost)
										{
											pHorizontalBoxCost->SetVisibility(ESlateVisibility::Visible);
											UOverlay *pUOverlay = Cast<UOverlay>(pHorizontalBoxCost->GetChildAt(1));
											if (pUOverlay)
											{
												UImage *pManaImg = Cast<UImage>(pUOverlay->GetChildAt(0));
												UImage *pCupsImg = Cast<UImage>(pUOverlay->GetChildAt(1));
												if (pManaImg && pCupsImg)
												{
													if (pUMyFeatureBase->OpenCost > 0)
													{
														pManaImg->SetVisibility(ESlateVisibility::Visible);
														pCupsImg->SetVisibility(ESlateVisibility::Hidden);
													}
													else
													{
														pManaImg->SetVisibility(ESlateVisibility::Hidden);
														pCupsImg->SetVisibility(ESlateVisibility::Visible);
														pText->SetText(FText::Format(LOCTEXT("Cost", "Cost: {0}"), pUMyFeatureBase->OpenCostCups));
													}
												}
											}
										}
									}

								}
							}
							if (0 < kk)
							{
								UWidget *pTitleWidget = pVerticalBox->GetChildAt(kk - 1);
								UTextBlock *pText = Cast<UTextBlock>(pTitleWidget);
								if (pText)
								{
									pUMyFeatureBase->InfoTitleText = pText->GetText();
								}
							}
						}
					}
				}
			}
			if (pUMyButtonLevel)
			{
				TArray<FLevelItemsChance> aFLevelItemsChanceSorted;
				for (const FLevelItemsChance &itr : pUMyButtonLevel->LevelSettingsCopy.LevelItemsChance)
				{
					bool bAdded = false;
					for (int32 iIndex = 0; iIndex < aFLevelItemsChanceSorted.Num(); ++iIndex)
					{
						if (itr.TileChance < aFLevelItemsChanceSorted[iIndex].TileChance)
						{
							aFLevelItemsChanceSorted.Insert(itr, iIndex);
							bAdded = true;
							break;
						}
					}
					if (!bAdded)
					{
						aFLevelItemsChanceSorted.Push(itr);
					}
				}
				pUMyButtonLevel->LevelSettingsCopy.LevelItemsChance = aFLevelItemsChanceSorted;
			}
			IsInPreviousRowAllElementsOpen = IsInThisRowAllElementsOpen;
		}
	}

	UButton *pUButton = WidgetTree->FindWidget<UButton>(TEXT("Button_Workshop"));
	if (pUButton)
	{
		if (ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.WorkshopEnabled)
		{
			pUButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			pUButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	UVerticalBox *pUVerticalBox = WidgetTree->FindWidget<UVerticalBox>(TEXT("VerticalBoxGetMana"));
	if (pUVerticalBox)
	{
		if (ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.ManaSource > 0)
		{ // VerticalBoxGetMana
			pUVerticalBox->SetIsEnabled(true);
			pManaProgressBar = WidgetTree->FindWidget<UProgressBar>(TEXT("ManaProgressBar"));
			pManaProgressBarText = WidgetTree->FindWidget<UTextBlock>(TEXT("ManaProgressBarText"));
			if (UMySaveGame::pUMySaveGame->iManaSourceTime == 0)
			{
				UMySaveGame::pUMySaveGame->iManaSourceTime = FDateTime::UtcNow().ToUnixTimestamp();
				UMySaveGame::SaveGame();
				UpdateManaProgressBar();
			}
			UButton *pButtonGetMana = WidgetTree->FindWidget<UButton>(TEXT("ButtonGetMana"));
			if (pButtonGetMana)
			{
				pButtonGetMana->OnClicked.AddUniqueDynamic(this, &UBreak2BricksGameMenu::OnButtonGetManaClicked); // Example click binding.
			}
		}
		else
		{
			pUVerticalBox->SetIsEnabled(false);
		}
	}

	UpdateCups();
}

void UBreak2BricksGameMenu::UpdateManaProgressBar()
{
	if (!pManaProgressBar)
	{
		return;
	}
	int64 iSeconds = FDateTime::UtcNow().ToUnixTimestamp();
	float fMinutes = (float)((iSeconds - UMySaveGame::pUMySaveGame->iManaSourceTime)/60.0f);
	if (fMinutes < 0)
	{
		check(false);
		fMinutes = 0;
		UMySaveGame::pUMySaveGame->iManaSourceTime = FDateTime::UtcNow().ToUnixTimestamp();
		UMySaveGame::SaveGame();
	}
	float fPercent = fMinutes / ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.ManaSource;
	if (fPercent > 1)
	{
		fPercent = 1;
	}
	pManaProgressBar->SetPercent(fPercent);
	pManaProgressBarText->SetText( FText::AsNumber( (int)(ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.ManaSource * fPercent) ) );
}

void UBreak2BricksGameMenu::Init()
{
	bClearSavedData = false;
	{/*
		UTextBlock *tstText = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
		tstText->SetText(FText::FromString(TEXT("Test Button")));
		UButton *tstBtn = NewObject<UButton>(this, UButton::StaticClass());
		tstBtn->AddChild(tstText);
		UPanelWidget *pPanel = WidgetTree->FindWidget<UPanelWidget>(TEXT("ScrollBoxLevels"));
		pPanel->AddChild(tstBtn);*/
	}
}

void UBreak2BricksGameMenu::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (sCurrentLanguage != UMySaveGame::pUMySaveGame->CurrentLanguage)
	{
		UMySaveGame::pUMySaveGame->CurrentLanguage = sCurrentLanguage;
		UMySaveGame::SaveGame();
		if (sCurrentLanguage == "English")
		{
			FInternationalization::Get().SetCurrentCulture("en");
		}
		else if (sCurrentLanguage == "Russian")
		{
			FInternationalization::Get().SetCurrentCulture("ru");
		}
	}
	UpdateManaProgressBar();
}

class UMyButtonLevel* UBreak2BricksGameMenu::GetPressedButtonLevel()
{
	UMyButtonLevel *pUMyButtonLevelPressed_ = pUMyButtonLevelPressed;
	pUMyButtonLevelPressed = nullptr;
	return pUMyButtonLevelPressed_;
}

class UMyButtonFeature* UBreak2BricksGameMenu::GetPressedButtonFeature()
{
	UMyButtonFeature *pUMyButtonFeaturePressed_ = pUMyButtonFeaturePressed;
	pUMyButtonFeaturePressed = nullptr;
	return pUMyButtonFeaturePressed_;
}

class UMyButtonLevel* UBreak2BricksGameMenu::GetPressedButtonLevelInfo()
{
	UMyButtonLevel *pUMyButtonLevelInfoPressed_ = pUMyButtonLevelInfoPressed;
	pUMyButtonLevelInfoPressed = nullptr;
	return pUMyButtonLevelInfoPressed_;
}

bool UBreak2BricksGameMenu::GetButtonWorkshopClicked()
{
	bool bButtonWorkshopClickedTmp = bButtonWorkshopClicked;
	bButtonWorkshopClicked = false;
	return bButtonWorkshopClickedTmp;
}

void UBreak2BricksGameMenu::OnButtonLevelClicked(UMyButtonLevel *pUMyButtonLevel)
{
	UScrollBox *pScrollBox = WidgetTree->FindWidget<UScrollBox>(TEXT("ScrollBoxLevels"));
	UMySaveGame::pUMySaveGame->SetMainMenuScrollOffset(pScrollBox->GetScrollOffset());
	pScrollBox->SetScrollOffset(UMySaveGame::pUMySaveGame->fMainMenuScrollOffset);
	pUMyButtonLevelPressed = pUMyButtonLevel;
}

void UBreak2BricksGameMenu::OnButtonFeatureClicked(class UMyButtonFeature *pUMyButtonFeature)
{
	pUMyButtonFeaturePressed = pUMyButtonFeature;
}

void UBreak2BricksGameMenu::OnButtonInfoClicked()
{
	UMyButtonLevel *pUMyButtonLevel = nullptr;
	if (pBtnInfo)
	{
		UPanelWidget *pWidgetParent = pBtnInfo->GetParent();
		UHorizontalBox *pHorizontalBox = Cast<UHorizontalBox>(pWidgetParent);
		if (pHorizontalBox)
		{
			bool IsInThisRowAllElementsOpen = true;
			for (int32 jj = 0; jj < pHorizontalBox->GetChildrenCount(); ++jj)
			{
				UWidget *pWidgetChild2 = pHorizontalBox->GetChildAt(jj);
				UVerticalBox *pVerticalBox = Cast<UVerticalBox>(pWidgetChild2);
				if (pVerticalBox)
				{
					for (int32 kk = 0; kk < pVerticalBox->GetChildrenCount(); ++kk)
					{
						pUMyButtonLevel = Cast<UMyButtonLevel>(pVerticalBox->GetChildAt(kk));
						if (pUMyButtonLevel)
						{
							break;
						}
					}
				}
				if (pUMyButtonLevel)
				{
					break;
				}
			}
		}
	}
	pUMyButtonLevelInfoPressed = pUMyButtonLevel;
}

void UBreak2BricksGameMenu::OnButtonWorkshopClicked()
{
	bButtonWorkshopClicked = true;
}

void UBreak2BricksGameMenu::OnButtonGetManaClicked()
{ /*
	float fMinutes = (float)((FPlatformTime::Seconds() - UMySaveGame::pUMySaveGame->dManaSourceTime) / 60.0);
	float fPercent = fMinutes / ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.ManaSource;
	if (fPercent > 1)
	{
		fPercent = 1;
	}
	pManaProgressBar->SetPercent(fPercent);
	pManaProgressBarText->SetText(FText::AsNumber((int)(ABreak2BricksBlockGrid::pABreak2BricksBlockGrid->GloablSettings.ManaSource * fPercent)));
*/
	int32 iMana = FCString::Atoi(*pManaProgressBarText->GetText().ToString());
	UMyHudWidget::AddMana(iMana);
	UMySaveGame::pUMySaveGame->iManaSourceTime = FDateTime::UtcNow().ToUnixTimestamp();
	UMySaveGame::SaveGame();
	UpdateManaProgressBar();
}

#undef LOCTEXT_NAMESPACE
