// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __BREAK2BRICKS_H__
#define __BREAK2BRICKS_H__


#include "EngineMinimal.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/UMG/Public/UMG.h"
#include "SlateBasics.h"
#include "Components/TextRenderComponent.h"
#include <algorithm>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <random>

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

namespace MyHelper
{
	// empty name for first actor
	template <typename T> bool GetActorFromScene(UWorld *pWorld, FString sActorName, T* &pOutActor)
	{
		for (TActorIterator<T> ActorItr(pWorld); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() == sActorName || sActorName.IsEmpty())
			{
				pOutActor = *ActorItr;
				return true;
			}
		}
		return false;
	}
}

struct MT19937
{
private:
	static std::random_device rd;
	static std::mt19937 rng;
public:
	// This is equivalent to srand().
	static void seed(uint64_t new_seed = std::mt19937::default_seed)
	{
		rng.seed(new_seed);
	}

	// This is equivalent to rand().
	static int32 get(int32 iRandMax);
};

#endif

