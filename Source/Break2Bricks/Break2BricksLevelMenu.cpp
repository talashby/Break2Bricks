// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "Break2BricksLevelMenu.h"
#include "MySaveGame.h"
#include "MyButtonLevel.h"


UBreak2BricksLevelMenu::UBreak2BricksLevelMenu() : UUserWidget(FObjectInitializer())
{
}

void UBreak2BricksLevelMenu::Init(const FLevelSettings &oFLevelSettings_)
{
	bRestartButtonPressed = false;
	bBackButtonPressed = false;
	bBombCheckbox = false;
	bColorBombCheckbox = false;
	oFLevelSettings = oFLevelSettings_;
	SetBombNum(oFLevelSettings.BombsNum);
	SetColorBombNum(oFLevelSettings.ColorBombsNum);
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
		sBombNum = FText();
	}
	else
	{
		SetVisibilityBombMenu(true);
	}
}

void UBreak2BricksLevelMenu::BurnBomb()
{
	bBombCheckbox = true;
	bColorBombCheckbox = false;
}

void UBreak2BricksLevelMenu::SetColorBombNum(int32 iBombNum_)
{
	iColorBombNum = iBombNum_;
	sColorBombNum = FText::FromString(FString::FromInt(iColorBombNum));
	if (0 == iColorBombNum)
	{
		SetVisibilityColorBombMenu(false);
		sColorBombNum = FText();
	}
	else
	{
		SetVisibilityColorBombMenu(true);
	}
}

void UBreak2BricksLevelMenu::BurnColorBomb()
{
	bColorBombCheckbox = true;
	bBombCheckbox = false;
}

void UBreak2BricksLevelMenu::DecColorBomb()
{
	if (0 < iColorBombNum)
	{
		SetColorBombNum(iColorBombNum - 1);
	}
}
