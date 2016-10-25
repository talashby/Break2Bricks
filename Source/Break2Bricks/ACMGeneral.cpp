// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGeneral.h"
#include "ACMGameMenu.h"
#include "Break2BricksPawn.h"
//#include "Break2BricksBlockGrid.h"

static const FName g_ssGameMenu("GameMenu");

ACMGeneral::ACMGeneral(ABreak2BricksPawn *owner) : ACMachine("ACMGeneral")
{
	pOwnerActor = owner;
    REGISTER_ACSTATE(ACMGeneral, GameMenu);
}

ACMGeneral::~ACMGeneral()
{
}

FName ACMGeneral::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
	{		
        return g_ssGameMenu; // ******************************* State Finished ********************************
    }
    return "";
}

FName ACMGeneral::TickStateGameMenu(int iTickType)
{
	if (ACMachine::TICK_StateStarted == iTickType)
	{
		spACMGameMenu = SP_ACMGameMenu(new ACMGameMenu(pOwnerActor));
	}
	else if (ACMachine::TICK_StateNormal == iTickType)
	{
		spACMGameMenu->TickPublic();
	}
	return "";
}
