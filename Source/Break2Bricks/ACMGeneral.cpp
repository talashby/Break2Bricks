// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGeneral.h"
#include "ACMPlayingField.h"
#include "Break2BricksPawn.h"

static const FName g_ssGame("Game");

ACMGeneral::ACMGeneral(ABreak2BricksPawn *owner) : ACMachine("ACMGeneral")
{
	pOwnerActor = owner;

    REGISTER_ACSTATE(ACMGeneral, Game);
}

ACMGeneral::~ACMGeneral()
{
}

FName ACMGeneral::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
        return g_ssGame;
    }
    return "";
}

FName ACMGeneral::TickStateGame(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
        spACMPlayingField = SP_ACMPlayingField(new ACMPlayingField(pOwnerActor));
    }
    else if (ACMachine::TICK_StateNormal == iTickType)
    {
        spACMPlayingField->TickPublic();
    }
    return "";
}
