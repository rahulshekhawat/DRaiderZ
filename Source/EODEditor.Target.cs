// Copyright 2018 Moikkai Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EODEditorTarget : TargetRules
{
	public EODEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "EOD", "EditorTools" } );
	}
}
