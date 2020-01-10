// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyButtonFeature.h"
#include "Break2BricksGameMenu.h"

#define LOCTEXT_NAMESPACE "UMG"

UMyButtonFeature::UMyButtonFeature() : UMyButtonFeatureBase()
{
	pUMyButtonLevel = nullptr;
	OnClicked.AddUniqueDynamic(this, &UMyButtonFeature::OnClick);
}

#if WITH_EDITOR
const FText UMyButtonFeature::GetPaletteCategory()
{
	return LOCTEXT("", "My Custom Stuff");
}
#endif

void UMyButtonFeature::Update()
{
	if (GetChildrenCount() > 0)
	{
		UWidget *pWidgetChild = GetChildAt(0);
		UTextBlock *pText = Cast<UTextBlock>(pWidgetChild);
		if (pText)
		{
			if (IsOpen)
			{
				pText->SetText(LOCTEXT("Info", "Info"));
			}
			else
			{
				pText->SetText(LOCTEXT("Open", "Open"));
			}
		}
	}
}

void UMyButtonFeature::OnClick()
{
	UBreak2BricksGameMenu::GetMenu()->OnButtonFeatureClicked(this);
}

#undef  LOCTEXT_NAMESPACE
