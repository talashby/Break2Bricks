// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "Break2BricksBlock.generated.h"

class ACMPlayingField;
/** A block that can be clicked */
UCLASS(minimalapi)
class ABreak2BricksBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	/** Grid that owns us */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABreak2BricksBlockGrid* OwningGrid;

public:
	ABreak2BricksBlock();
	void Init(ACMPlayingField *pOwnerACM_, ABreak2BricksBlockGrid *pOwnerGrid_, int32 iType_, int32 iX, int32 iY);

	/** Is drop down animation played */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bAnimDropDown;

	/** Is drop down animation played */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bAnimConnectColumns;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION(Category = "Animation", BlueprintImplementableEvent, BlueprintCallable)
	void AnimDropDown(int32 iSteps);

	UFUNCTION(Category = "Animation", BlueprintImplementableEvent, BlueprintCallable)
	void AnimConnectColumns(const FVector vecNewLocation);

	void HandleClicked();

	void Highlight(bool bOn);

	int32 GetXPos() const { return iXPos; }
	void SetXPos(int32 iXPos_) { iXPos = iXPos_; }

	int32 GetYPos() const { return iYPos; }
	void SetYPos(int32 iYPos_) { iYPos = iYPos_; }

	int32 GetType() const { return iType; }

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

private:
	int32 iType;
	int32 iXPos;
	int32 iYPos;

	ACMPlayingField *pOwnerACM;
};



