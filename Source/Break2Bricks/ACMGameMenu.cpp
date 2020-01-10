// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGameMenu.h"
#include "ACMPlayingField.h"
#include "Break2BricksPawn.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksGameMenu.h"
#include "Break2BricksGameMode.h"
#include "Break2BricksLevelMenu.h"
#include "EngineUtils.h"
#include "MySaveGame.h"
#include "MyButtonLevel.h"
#include "MyButtonFeature.h"
#include "MyModalDialog.h"
#include "MyModalWorkshopDialog.h"
#include "MyHudWidget.h"

//#define LOCTEXT_NAMESPACE "GameMenu" 

static const FName g_ssGameMenu("GameMenu");
static const FName g_ssModalDialog("ModalDialog");
static const FName g_ssModalDialogLevelInfo("ModalDialogLevelInfo");
static const FName g_ssModalDialogWorkshop("ModalDialogWorkshop");
static const FName g_ssModalDialogUpgrade("ModalDialogUpgrade");
static const FName g_ssFirstStart("FirstStart");
static const FName g_ssGame("Game");

ACMGameMenu::ACMGameMenu(ABreak2BricksPawn *owner) : ACMachine("ACMGameMenu")
{
	pOwnerActor = owner;
	pGameMenu = nullptr;
	pGridActor = nullptr;
    REGISTER_ACSTATE(ACMGameMenu, GameMenu);
	REGISTER_ACSTATE(ACMGameMenu, ModalDialog);
	REGISTER_ACSTATE(ACMGameMenu, ModalDialogLevelInfo);
	REGISTER_ACSTATE(ACMGameMenu, ModalDialogWorkshop);
	REGISTER_ACSTATE(ACMGameMenu, ModalDialogUpgrade);
	REGISTER_ACSTATE(ACMGameMenu, FirstStart);
	REGISTER_ACSTATE(ACMGameMenu, Game);
}

ACMGameMenu::~ACMGameMenu()
{
}

FName ACMGameMenu::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
		TActorIterator<ABreak2BricksBlockGrid> ActorItr = TActorIterator<ABreak2BricksBlockGrid>(pOwnerActor->GetWorld());
		if (ActorItr)
		{
			pGridActor = *ActorItr;
		}
		else
		{
			return ErrorState("ABreak2BricksBlockGrid not found"); // ******************************* State Finished ********************************
		}
		if (nullptr == pGameMenu)
		{
			pGameMenu = pGridActor->pGameMenu;
			if (nullptr == pGameMenu)
			{
				return ErrorState("UBreak2BricksGameMenu not found"); // ******************************* State Finished ********************************
			}
		}
		
        return g_ssGameMenu; // ******************************* State Finished ********************************
    }
    return "";
}

FName ACMGameMenu::TickStateGameMenu(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		if (pGameMenu)
		{
			if (!pGameMenu->IsInViewport())
			{
				pGameMenu->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Back);
			}
			if (APlayerController* pPC = Cast<APlayerController>(pOwnerActor->GetController()))
			{
				FInputModeGameAndUI Mode;
				Mode.SetWidgetToFocus(pGameMenu->TakeWidget());
				pPC->SetInputMode(Mode);
			}
			pGameMenu->Init();
		}
		else
		{
			return ErrorState("Widget not found");
		}
		
		
		/*TActorIterator<ABreak2BricksGameMenu> ActorItr = TActorIterator<ABreak2BricksGameMenu>(pOwnerActor->GetWorld());
		if (ActorItr)
		{
			pGameMenuActor = *ActorItr;
		}
		else
		{
			return ErrorState("ABreak2BricksGameMenu not found"); // ******************************* State Finished ********************************
		}
		pGameMenuActor->SetActorHiddenInGame(false);
		pGameMenuActor->pACMGameMenu = this;*/
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		pUMyButtonPressed = pGameMenu->GetPressedButtonLevelInfo();
		if (nullptr != pUMyButtonPressed)
		{
			return g_ssModalDialogLevelInfo; // ******************************* State Finished ********************************
		}

		pUMyButtonPressed = pGameMenu->GetPressedButtonLevel();
		if (nullptr != pUMyButtonPressed)
		{
			if (pUMyButtonPressed->IsOpen)
			{
				if (UMySaveGame::pUMySaveGame->FirstStart)
				{
					UMySaveGame::pUMySaveGame->FirstStart = false;
					return g_ssFirstStart; // ******************************* State Finished ********************************
				}
				if (UMySaveGame::pUMySaveGame->DifficultLevelWarning && ((UMyButtonLevel*)pUMyButtonPressed)->DifficultLevel)
				{
					UMySaveGame::pUMySaveGame->DifficultLevelWarning = false;
					return g_ssFirstStart; // ******************************* State Finished ********************************
				}
				return g_ssGame; // ******************************* State Finished ********************************
			}
			else
			{
				return g_ssModalDialog; // ******************************* State Finished ********************************
				//FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Are you shure?", "Are you shure?"));
			}
			/*if (pGameMenu->GetMana() >= 10)
			{
				pGameMenu->AddMana(-10);
				return g_ssGame;
			}*/
		}
		pUMyButtonPressed = pGameMenu->GetPressedButtonFeature();
		if (nullptr != pUMyButtonPressed)
		{
			return g_ssModalDialog; // ******************************* State Finished ********************************
		}
		if (true == pGameMenu->bClearSavedData)
		{
			pGameMenu->bClearSavedData = false;
			ABreak2BricksGameMode *pB2BGameMode = Cast<ABreak2BricksGameMode>(pOwnerActor->GetWorld()->GetAuthGameMode());
			pB2BGameMode->ClearSavedGame();
			pGameMenu->InitOnce();
			pGameMenu->Init();
		}
		if (pGameMenu->GetButtonWorkshopClicked())
		{
			return g_ssModalDialogWorkshop;
		}
	}
	return "";
}

FName ACMGameMenu::TickStateModalDialog(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		ShowModalDialog(pUMyButtonPressed);
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pGridActor->pModalDialog->bBackButtonPressed)
		{
			pGridActor->pModalDialog->RemoveFromViewport();
			return g_ssGameMenu; // ******************************* State Finished ********************************
		}
		if (pGridActor->pModalDialog->bOKButtonPressed)
		{
			if (pUMyButtonPressed && !pUMyButtonPressed->IsOpen)
			{
				UMyButtonLevel *pUMyButtonLevel = Cast<UMyButtonLevel>(pUMyButtonPressed);
				if (!pUMyButtonLevel)
				{
					UMyButtonFeature *pUMyButtonFeature = Cast<UMyButtonFeature>(pUMyButtonPressed);
					check(pUMyButtonFeature);
					if (pUMyButtonFeature)
					{
						pUMyButtonLevel = pUMyButtonFeature->pUMyButtonLevel;
					}
				}

				bool bIsOpened = false;
				if (pUMyButtonPressed->OpenCost > 0)
				{
					if (UMyHudWidget::GetMana() >= pUMyButtonPressed->OpenCost)
					{
						bIsOpened = true;
						UMyHudWidget::AddMana(-pUMyButtonPressed->OpenCost);
						UMyHudWidget::Update();
					}
				}
				else if (pUMyButtonPressed->OpenCostCups > 0)
				{
					if (UMySaveGame::pUMySaveGame->aCups[pUMyButtonPressed->LevelIndex] >= pUMyButtonPressed->OpenCostCups)
					{
						bIsOpened = true;
						UMySaveGame::pUMySaveGame->aCups[pUMyButtonPressed->LevelIndex] -= pUMyButtonPressed->OpenCostCups;
						UBreak2BricksGameMenu::GetMenu()->UpdateCups();
					}
				}

				if (bIsOpened)
				{
					pUMyButtonPressed->IsOpen = true;
					UMySaveGame::pUMySaveGame->OpenButton(pUMyButtonLevel->LevelIndex, pUMyButtonPressed->GetName());
					UMySaveGame::SaveGame();
					pGameMenu->InitOnce();
					pGameMenu->Init();
				}
			}
			pGridActor->pModalDialog->RemoveFromViewport();
			return g_ssGameMenu; // ******************************* State Finished ********************************
		}
	}
	return "";
}

FName ACMGameMenu::TickStateModalDialogLevelInfo(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		UMyButtonLevel *pUMyButtonLevel = Cast<UMyButtonLevel>(pUMyButtonPressed);
		ShowLevelInfo(pUMyButtonLevel);
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pGridActor->pModalDialogLevelInfo->bBackButtonPressed)
		{
			pGridActor->pModalDialogLevelInfo->RemoveFromViewport();
			return g_ssGameMenu; // ******************************* State Finished ********************************
		}
	}
	return "";
}

FName ACMGameMenu::TickStateModalDialogWorkshop(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		if (pGridActor->pModalDialogWorkshop)
		{
			pGridActor->pModalDialogWorkshop->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
			pGridActor->pModalDialogWorkshop->InitWorkshop();
		}
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pGridActor->pModalDialogWorkshop->bBackButtonPressed)
		{
			pGridActor->pModalDialogWorkshop->RemoveFromViewport();
			return g_ssGameMenu; // ******************************* State Finished ********************************
		}
		else if (pGridActor->pModalDialogWorkshop->GetUpgradeBtnPressed())
		{
			iModalDialogUpgrade_SelectedItem = pGridActor->pModalDialogWorkshop->GetSelectedItem();
			pGridActor->pModalDialogWorkshop->RemoveFromViewport();
			return g_ssModalDialogUpgrade;
		}
	}
	return "";
}

FName ACMGameMenu::TickStateModalDialogUpgrade(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		if (pGridActor->pModalDialogUpgrade)
		{
			pGridActor->pModalDialogUpgrade->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
			pGridActor->pModalDialogUpgrade->InitUpgradeScreen(iModalDialogUpgrade_SelectedItem);
		}
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pGridActor->pModalDialogUpgrade->bBackButtonPressed)
		{
			pGridActor->pModalDialogUpgrade->RemoveFromViewport();
			return g_ssModalDialogWorkshop; // ******************************* State Finished ********************************
		}
	}
	return "";
}

FName ACMGameMenu::TickStateFirstStart(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		pOwnerActor->ResetAnyClick();
		UMyButtonLevel *pUMyButtonLevel = Cast<UMyButtonLevel>(pUMyButtonPressed);
		ShowLevelInfo(pUMyButtonLevel, false);
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (pOwnerActor->IsAnyClick())
		{
			pGridActor->pModalDialogLevelInfo->RemoveFromViewport();
			return g_ssGame; // ******************************* State Finished ********************************
		}
	}
	return "";
}

FName ACMGameMenu::TickStateGame(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
		UMyButtonLevel *pUMyButtonLevel = Cast<UMyButtonLevel>(pUMyButtonPressed);
        spACMPlayingField = SP_ACMPlayingField(new ACMPlayingField(pOwnerActor, pUMyButtonLevel->LevelSettingsCopy, pUMyButtonLevel->WinningPrize));
		pGameMenu->RemoveFromViewport();
    }
    else if (ACMachine::TICK_StateNormal == iTickType)
    {
        spACMPlayingField->TickPublic();
		if (spACMPlayingField->IsGameFinished())
		{
			if (spACMPlayingField->IsLevelComplete())
			{
				UMyButtonLevel *pUMyButtonLevel = Cast<UMyButtonLevel>(pUMyButtonPressed);
				if (pUMyButtonLevel)
				{
					int32 iManaPrize = pGridActor->GetManaPrize(&pUMyButtonLevel->WinningPrize);
					UMyHudWidget::AddMana(iManaPrize);
					++UMySaveGame::pUMySaveGame->aCups[pUMyButtonLevel->LevelIndex];
					UBreak2BricksGameMenu::GetMenu()->UpdateCups();
				}
				UMySaveGame::SaveGame();
			}
			spACMPlayingField.Reset();
			return GetStateStartName(); // ******************************* State Finished ********************************
		}
        if (pGridActor->pLevelMenu->bBackButtonPressed)
        {
            pGridActor->pLevelMenu->RemoveFromViewport();
            spACMPlayingField.Reset();
            return GetStateStartName(); // ******************************* State Finished ********************************
        }
        if (pGridActor->pLevelMenu->bRestartButtonPressed)
        {
            spACMPlayingField.Reset();
			pGridActor->pLevelMenu->RemoveFromViewport();
            return g_ssGame; // ******************************* State Restarted ********************************
        }
    }
    return "";
}

void ACMGameMenu::ShowModalDialog(UMyButtonFeatureBase *pUMyButtonFeature_)
{
	if (pGridActor->pModalDialog)
	{
		pGridActor->pModalDialog->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
		pGridActor->pModalDialog->Init(pUMyButtonFeature_);
		if (APlayerController* pPC = Cast<APlayerController>(pOwnerActor->GetController()))
		{
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(pGridActor->pModalDialog->TakeWidget());
			pPC->SetInputMode(Mode);
		}
	}
}

void ACMGameMenu::ShowLevelInfo(UMyButtonLevel *pUMyButtonLevel_, bool bBackButtonVisible) const
{
	if (pGridActor->pModalDialogLevelInfo)
	{
		pGridActor->pModalDialogLevelInfo->AddToViewport(ABreak2BricksBlockGrid::WidgetZOrder_Modal);
		pGridActor->pModalDialogLevelInfo->InitLevelInfo(pUMyButtonLevel_, bBackButtonVisible);
	}
}
//#undef LOCTEXT_NAMESPACE