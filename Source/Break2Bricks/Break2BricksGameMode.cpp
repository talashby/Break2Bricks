// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"
#include "Break2BricksGameMode.h"
#include "Break2BricksPlayerController.h"
#include "Break2BricksPawn.h"

ABreak2BricksGameMode::ABreak2BricksGameMode()
{
	// no pawn by default
	DefaultPawnClass = ABreak2BricksPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ABreak2BricksPlayerController::StaticClass();
}
