// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "GameField.h"


// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	pName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("UTextRenderComponent0"));
	pName->SetupAttachment(DummyRoot);
	pName->SetRelativeRotation(FRotator(90, 0, -90));
	pName->SetRelativeScale3D(FVector(2, 2, 2));
	pName->SetHorizontalAlignment(EHTA_Center);
	pName->SetText(FText::FromString("GameField"));
	pName->SetTextRenderColor(FColor(0, 100, 255));
	pName->SetHiddenInGame(true);
}

AGameField::~AGameField()
{
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameField::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

