// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __BREAK2BRICKS_H__
#define __BREAK2BRICKS_H__


#include "EngineMinimal.h"
#include "SlateBasics.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogGeneral, Log, All);
namespace LogHelper
{
    bool CheckLogLevel(int iLogSeverityLevel);
}

template <typename VictoryObjType>
static FORCEINLINE VictoryObjType* SpawnBP(
	UWorld*         TheWorld,
	UClass*         TheBP,
	const FVector&  Loc,
	const FRotator& Rot,
	AActor*         Owner = NULL,
	APawn*          Instigator = NULL
	) {
	if (!TheWorld) return NULL;
	if (!TheBP) return NULL;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = Owner;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.bDeferConstruction = false;

	return TheWorld->SpawnActor<VictoryObjType>(TheBP, Loc, Rot, SpawnInfo);
}

#endif
