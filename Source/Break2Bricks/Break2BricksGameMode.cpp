// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksGameMode.h"
#include "Break2BricksPlayerController.h"
#include "Break2BricksPawn.h"
#include "MySaveGame.h"
#include "MyHudWidget.h"

ABreak2BricksGameMode::ABreak2BricksGameMode()
{
	// no pawn by default
	DefaultPawnClass = ABreak2BricksPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ABreak2BricksPlayerController::StaticClass();
}

void ABreak2BricksGameMode::InitGameState()
{
	LoadGameInstance = UMySaveGame::LoadGame();
	//FInternationalization::Get().SetCurrentCulture("ru");
}

void ABreak2BricksGameMode::ClearSavedGame()
{
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	UMySaveGame::pUMySaveGame = LoadGameInstance;
	UMySaveGame::SaveGame();
	UMyHudWidget::GetMenu()->Init();
}

void ABreak2BricksGameMode::B2BAddMana()
{
	UMySaveGame::pUMySaveGame->bDebugMaxMana = true;
	UMyHudWidget::AddMana(200);
	UMySaveGame::SaveGame();
}
