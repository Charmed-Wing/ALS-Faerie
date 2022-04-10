using UnrealBuildTool;

public class ALS : ModuleRules
{
	public ALS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "NetCore", "Engine", "PhysicsCore", "GameplayTags", "AnimGraphRuntime", "ControlRig",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"RigVM"
		});
	}
}