// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "ACMGeneral.h"

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
    return g_ssGame;
}

FName ACMGeneral::TickStateGame()
{
    return "";
}
