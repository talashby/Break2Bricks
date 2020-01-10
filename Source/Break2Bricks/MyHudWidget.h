// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MyHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UMyHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMyHudWidget();
	static UMyHudWidget* GetMenu();
	void Init();

	static int32 GetMana();
	static void SetMana(int32 iMana);
	static void AddMana(int32 iMana);
	static void Update();

	UPROPERTY(Category = HUD, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText Mana;
	
	UPROPERTY(Category = HUD, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText MaxMana;
private:

	//void ConnectToChatServerCompleted(const class FXmppUserJid &oJid, bool bSuccess, const FString &sError);
};
