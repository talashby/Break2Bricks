// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "Break2BricksGameMenu.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "GameMenu"

// Sets default values
ABreak2BricksGameMenu::ABreak2BricksGameMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	pDummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = pDummyRoot;

	// Create static mesh component
	pManaText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	pManaText->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
	pManaText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	pManaText->SetText(FText::Format(LOCTEXT("ManaFmt", "Mana: {0}"), FText::AsNumber(100)));
	pManaText->SetupAttachment(pDummyRoot);
}

// Called when the game starts or when spawned
void ABreak2BricksGameMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreak2BricksGameMenu::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

