// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "GeneralSettings.h"


// Sets default values
AGeneralSettings::AGeneralSettings()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	UTextRenderComponent *pName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("UTextRenderComponent0"));
	pName->SetupAttachment(DummyRoot);
	pName->SetRelativeRotation(FRotator(90, 0, -90));
	pName->SetRelativeScale3D(FVector(2, 2, 2));
	pName->SetHorizontalAlignment(EHTA_Center);
	pName->SetText(FText::FromString("GeneralSettings"));
	pName->SetTextRenderColor(FColor(255, 100, 0));
	pName->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AGeneralSettings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneralSettings::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

