// Copyright 2018 Moikkai Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EODTarget : TargetRules
{
	public EODTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "EOD" } );
	}
}
