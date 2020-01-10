// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyButtonLevel.h"
#include "Break2BricksGameMenu.h"

#define LOCTEXT_NAMESPACE "UMG"

UMyButtonLevel::UMyButtonLevel() : UMyButtonFeatureBase()
{
	PlayCost = 0;
	DifficultLevel = false;
	OnClicked.AddUniqueDynamic(this, &UMyButtonLevel::OnClick);
}

void UMyButtonLevel::OnClick()
{
	UBreak2BricksGameMenu::GetMenu()->OnButtonLevelClicked(this);
}

void UMyButtonLevel::Update()
{
	if (GetChildrenCount() > 0)
	{
		UWidget *pWidgetChild = GetChildAt(0);
		UTextBlock *pText = Cast<UTextBlock>(pWidgetChild);
		if (pText)
		{
			if (IsOpen)
			{
				pText->SetText(LOCTEXT("Start", "Start"));
			}
			else
			{
				pText->SetText(LOCTEXT("Open", "Open"));
			}
		}
	}
}

#if WITH_EDITOR
const FText UMyButtonLevel::GetPaletteCategory()
{
	return LOCTEXT("", "My Custom Stuff");
}
#endif

#undef  LOCTEXT_NAMESPACE
