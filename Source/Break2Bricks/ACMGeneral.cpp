// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGeneral.h"
#include "ACMPlayingField.h"
#include "Break2BricksPawn.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksGameMenu.h"
//#include "Break2BricksGameMenu.h"
#include "EngineUtils.h"

static const FName g_ssGameMenu("GameMenu");
static const FName g_ssGame("Game");

ACMGeneral::ACMGeneral(ABreak2BricksPawn *owner) : ACMachine("ACMGeneral")
{
	pOwnerActor = owner;
	pGameMenu = nullptr;
	pGridActor = nullptr;
    REGISTER_ACSTATE(ACMGeneral, GameMenu);
	REGISTER_ACSTATE(ACMGeneral, Game);
}

ACMGeneral::~ACMGeneral()
{
}

void ACMGeneral::StartMenuStartGameButtonClicked()
{
	SetStateExternalSignal(EXTERNAL_SIGNAL_1);
}

FName ACMGeneral::TickStateStart(int iTickType)
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
		else
		{
			pGameMenu->Init();
		}
		
        return g_ssGameMenu; // ******************************* State Finished ********************************
    }
    return "";
}

FName ACMGeneral::TickStateGameMenu(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		if (pGameMenu)
		{
			pGameMenu->AddToViewport();
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
		pGameMenuActor->pACMGeneral = this;*/
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		if (true == pGameMenu->bPlayStandardGamePressed)
		{
			pGameMenu->bPlayStandardGamePressed = false;
			if (pGameMenu->GetMana() >= 10)
			{
				pGameMenu->AddMana(-10);
				return g_ssGame;
			}
		}
		if (true == pGameMenu->bDebugNextDayButtonPressed)
		{
			pGameMenu->bDebugNextDayButtonPressed = false;
			pGameMenu->AddMana(100);
		}
	}
	return "";
}

FName ACMGeneral::TickStateGame(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
        spACMPlayingField = SP_ACMPlayingField(new ACMPlayingField(pOwnerActor));
		pGameMenu->RemoveFromViewport();
    }
    else if (ACMachine::TICK_StateNormal == iTickType)
    {
        spACMPlayingField->TickPublic();
		if (spACMPlayingField->IsGameFinished())
		{
			spACMPlayingField.Reset();
			return GetStateStartName(); // ******************************* State Finished ********************************
		}
    }
    return "";
}
