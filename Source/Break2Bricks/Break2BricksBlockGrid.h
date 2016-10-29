// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include <vector>
#include "Break2BricksBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABreak2BricksBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* pNoMoreMovesText;

public:
	ABreak2BricksBlockGrid();

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 SizeX;
	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 SizeY;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	std::vector< TSubclassOf<class ABreak2BricksBlock> > aABPBlocks; // blueprint classes

	UPROPERTY()
	class UBreak2BricksGameMenu *pGameMenu;
	TSubclassOf<class UBreak2BricksGameMenu> pGameMenuClass; // blueprint class

	UPROPERTY()
	class UBreak2BricksLevelMenu *pLevelMenu;
	TSubclassOf<class UBreak2BricksLevelMenu> pLevelMenuClass; // blueprint class

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns NoMoreMovesText subobject **/
	USceneComponent* GetNoMoreMovesText() const;
};



