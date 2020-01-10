#pragma once

#include "LevelSettings.generated.h"

USTRUCT(BlueprintType)
struct FLevelItemsChance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<class ABreak2BricksBlock> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (UIMin = "0", UIMax = "1"))
	float TileChance;
};

USTRUCT(BlueprintType)
struct FLevelSettings
{
	GENERATED_BODY()

	FLevelSettings();

	void Add(const FLevelSettings &oFLevelSettings);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	int32 TileTypesNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	int32 BombsNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	int32 ColorBombsNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	bool ManaBonus;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints", meta = (UIMin = "0", UIMax = "1"))
	float ManaBonusChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	TArray<FLevelItemsChance> LevelItemsChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	bool AutoAddTiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	bool AutoAddTilesRandomColumn;
};

USTRUCT(BlueprintType)
struct FGlobalSettings
{
	GENERATED_BODY()

	FGlobalSettings()
	{
		WorkshopEnabled = false;
		ManaSource = 0;
		MaxMana1 = 0;
	}

	void Add(const FGlobalSettings &oFGloablSettings)
	{
		WorkshopEnabled |= oFGloablSettings.WorkshopEnabled;
		if (oFGloablSettings.ManaSource > ManaSource)
		{
			ManaSource = oFGloablSettings.ManaSource;
		}
		MaxMana1 += oFGloablSettings.MaxMana1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool WorkshopEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 ManaSource;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 MaxMana1;
};

USTRUCT(BlueprintType)
struct FWinningPrize
{
	GENERATED_BODY()

	FWinningPrize()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	int32 Mana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blueprints")
	TArray<int32> ManaForSavedBombs;
};