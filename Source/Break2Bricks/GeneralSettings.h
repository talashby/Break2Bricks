// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GeneralSettings.generated.h"

UCLASS()
class AGeneralSettings : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneralSettings();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:

	/** Dummy root component */
	UPROPERTY(Category = Components, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Is GameField opened or closed */
	UPROPERTY(Category = GeneralSettings, EditAnywhere, BlueprintReadOnly)
	TArray<class AGameField*> GameFields;
};
