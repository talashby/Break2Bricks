// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "Break2BricksGameMenu.h"


#define LOCTEXT_NAMESPACE "GameMenu" 

UBreak2BricksGameMenu::UBreak2BricksGameMenu() : UUserWidget(FObjectInitializer())
{
	//sCurrentMana = LOCTEXT("ManaFmt", "Mana: 90");
	iMana = 0;
	AddMana(100);
	sCurrentMana = LOCTEXT("ManaTMP", "Mana: 100");
	Init();
}

void UBreak2BricksGameMenu::Init()
{
	bPlayStandardGamePressed = false;
	bDebugNextDayButtonPressed = false;
}

void UBreak2BricksGameMenu::AddMana(int32 iManaAdd)
{
	iMana += iManaAdd;
	if (iMana < 0)
	{
		iMana = 0;
	}
	sCurrentMana = FText::Format(LOCTEXT("ManaFmt", "Mana: {0}"), FText::AsNumber(iMana));
}
