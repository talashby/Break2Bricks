// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FGameMenuSettings
{
	GENERATED_BODY()
	FGameMenuSettings()
	{
	}

	bool FindName(const FString &sFindingName)
	{
		for (FString sName : ButtonNames)
		{
			if (sFindingName == sName)
			{
				return true;
			}
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<FString> ButtonNames;
};
/**
 * 
 */
UCLASS()
class BREAK2BRICKS_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	static UMySaveGame* LoadGame();
	static void SaveGame();
	static UMySaveGame *pUMySaveGame;

	void OpenButton(int iLevelIndex, const FString &sButtonName);
	int32 GetMaxMana();
	bool IsAllowedDetonator() const;
	void SetMainMenuScrollOffset(float fMainMenuScrollOffset_);

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 LevelsPassed;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 Mana;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray< FGameMenuSettings > GameMenuSettings;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString CurrentLanguage;

	enum ITEMS
	{
		ITEMSManaBottle = 0,
		ITEMSBombAutoburn,
		ITEMSBigManaBottle,
		ITEMSGreatManaBottle,
		ITEMSRemoteDetonator,
		ITEMS_NUM
	};

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int32> aItems;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int32> aCups;
	/*UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 iItemBombAutoburnNum;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 iItemMaxManaNum;*/

	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool bDebugMaxMana;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool FirstStart;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool DifficultLevelWarning;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 iWorkshopSelectedBox;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int64 iManaSourceTime;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	float fMainMenuScrollOffset;
};
