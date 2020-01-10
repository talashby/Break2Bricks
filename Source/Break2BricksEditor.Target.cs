// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Break2BricksEditorTarget : TargetRules
{
    public Break2BricksEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Break2Bricks");
	}
}
