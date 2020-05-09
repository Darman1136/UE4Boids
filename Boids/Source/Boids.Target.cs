using UnrealBuildTool;
using System.Collections.Generic;

public class BoidsTarget : TargetRules
{
	public BoidsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Boids" } );
	}
}
