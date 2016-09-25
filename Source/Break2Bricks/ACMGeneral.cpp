// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGeneral.h"
#include "ACMPlayingField.h"

static const FName g_ssGame("Game");

ACMGeneral::ACMGeneral() : ACMachine("ACMGeneral")
{
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
        spACMPlayingField = SP_ACMPlayingField(new ACMPlayingField());
    }
    else if (ACMachine::TICK_StateNormal == iTickType)
    {
        spACMPlayingField->TickPublic();
    }
    return "";
}
