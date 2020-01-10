// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "LevelSettings.h"

#include "Break2BricksBlockGrid.generated.h"

USTRUCT(BlueprintType)
struct FItemsSettings
{
	GENERATED_BODY()

	FItemsSettings()
	{
		Texture = nullptr;
		SellManaCost = 1;
		UpgradeCost = 0;
		UpgradeItem = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<class ABreak2BricksBlock> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UTexture2D *Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText Definition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 SellManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 UpgradeCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 UpgradeItem;
};

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABreak2BricksBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:
	ABreak2BricksBlockGrid();
	static ABreak2BricksBlockGrid *pABreak2BricksBlockGrid;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeX;
	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeY;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	TArray< TSubclassOf<class ABreak2BricksBlock> > Tiles;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABreak2BricksBlock> TileManaBonus;
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	UTexture2D *TileManaBonusTexture;

	/*UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABreak2BricksBlock> TileMaxMana;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABreak2BricksBlock> TileBombAutoburn;*/

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadWrite)
	int32 InitialMaxMana;

	UPROPERTY(Category = Items, EditAnywhere, BlueprintReadWrite)
	int32 ItemMaxMana;

	UPROPERTY(Category = Items, EditAnywhere, BlueprintReadWrite)
	int32 ItemMaxManaBig;

	UPROPERTY(Category = Items, EditAnywhere, BlueprintReadWrite)
	int32 ItemMaxManaGreat;

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	int32 GetManaPrize(const FWinningPrize *pFWinningPrize);
	void GetPrize(const FWinningPrize *pFWinningPrize, int32 iIndex, int32 &iNum, UTexture2D *&pTexture);

	int GetItemSetting(TSubclassOf<class ABreak2BricksBlock> TileClass);

	enum WidgetZOrder
	{
		WidgetZOrder_Back = 0,
		WidgetZOrder_Modal,
		WidgetZOrder_Hud
	};

	UPROPERTY()
	class UBreak2BricksGameMenu *pGameMenu;
	TSubclassOf<class UBreak2BricksGameMenu> pGameMenuClass; // blueprint class

	UPROPERTY()
	class UBreak2BricksLevelMenu *pLevelMenu;
	TSubclassOf<class UBreak2BricksLevelMenu> pLevelMenuClass; // blueprint class

	UPROPERTY()
	class UMyModalDialog *pModalDialog;
	TSubclassOf<class UMyModalDialog> pModalDialogClass; // blueprint class

	UPROPERTY()
	class UMyModalDialog *pModalDialogFinishLevel;
	TSubclassOf<class UMyModalDialog> pModalDialogFinishLevelClass; // blueprint class

	UPROPERTY()
	class UMyModalDialog *pModalDialogLevelInfo;
	TSubclassOf<class UMyModalDialog> pModalDialogLevelInfoClass; // blueprint class

	UPROPERTY()
	class UMyModalWorkshopDialog *pModalDialogWorkshop;
	TSubclassOf<class UMyModalWorkshopDialog> pModalDialogWorkshopClass; // blueprint class

	UPROPERTY()
	class UMyModalDialog *pModalDialogUpgrade;
	TSubclassOf<class UMyModalDialog> pModalDialogUpgradeClass; // blueprint class

	UPROPERTY()
	class UMyHudWidget *pHUDWidget;
	TSubclassOf<class UMyHudWidget> pHUDWidgetClass; // blueprint class

	FGlobalSettings GloablSettings;

	UPROPERTY(Category = Items, EditAnywhere, BlueprintReadWrite)
	TArray<FItemsSettings> ItemsSettings;

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
};



