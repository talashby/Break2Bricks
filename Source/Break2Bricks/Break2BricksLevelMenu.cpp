// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "Break2BricksLevelMenu.h"




UBreak2BricksLevelMenu::UBreak2BricksLevelMenu() : UUserWidget(FObjectInitializer())
{
	Init();
}

void UBreak2BricksLevelMenu::Init()
{
	bBombCheckbox = false;
	SetBombNum(1);
}

void UBreak2BricksLevelMenu::DecBomb()
{
	if (0 < iBombNum)
	{
		SetBombNum(iBombNum - 1);
	}
}

void UBreak2BricksLevelMenu::SetBombNum(int32 iBombNum_)
{
	iBombNum = iBombNum_;
	sBombNum = FText::FromString(FString::FromInt(iBombNum));
	if (0 == iBombNum)
	{
		SetVisibilityBombMenu(false);
	}
	else
	{
		SetVisibilityBombMenu(true);
	}
}
