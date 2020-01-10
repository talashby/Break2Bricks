// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameField.generated.h"

UCLASS()
class AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameField();
	virtual ~AGameField();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:

	/** Dummy root component */
	UPROPERTY(Category = Components, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	UPROPERTY(Category = Components, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent *pName;

	/** Is GameField opened or closed */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	bool IsOpen;

	/** Cost for open */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	int32 OpenCost;

	/** Cost for one game */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	int32 PlayCost;

	/** Number of tile types */
	UPROPERTY(Category = GameField, EditAnywhere, BlueprintReadOnly)
	uint8 TileTypesNum;

private:
};
