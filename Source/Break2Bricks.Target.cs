// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Break2BricksTarget : TargetRules
{
    public Break2BricksTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Break2Bricks");
	}
}
