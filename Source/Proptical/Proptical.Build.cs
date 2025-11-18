using UnrealBuildTool;

public class Proptical : ModuleRules
{
	public Proptical(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Sockets",
			"Networking"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Networking modules for socket operations
		});
	}
}

