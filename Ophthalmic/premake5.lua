project "Ophthalmic"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"%{wks.location}/DAGGer/DAGGer/vendor/spdlog/include",
		"%{wks.location}/DAGGer/DAGGer/src",
		"%{wks.location}/DAGGer/DAGGer/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.DAALS}",

		"%{IncludeDir.Config}",
		"%{wks.location}"
	}
	
	links
	{
		"DAGGer"
	}
	
	filter {"system:windows"}			--	WINDOWS
		systemversion "latest"

	filter {"system:linux"}				--	LINUX
		systemversion "latest"

	filter {"system:macosx"}			--	MACOS
		systemversion "latest"
		
	filter "configurations:Debug"	--	DEBUG
		defines "OP_DEBUG"
		defines "Dr_DEBUG"
		runtime "Debug"
		symbols "on"
		
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}
		
	filter "configurations:Release"	--	RELEASE
		defines "OP_RELEASE"
		defines "Dr_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"	--	DISTRIBUTION
		defines "OP_DISTRIBUTION"
		defines "Dr_DISTRIBUTION"
		runtime "Release"
		optimize "on"
	filter ""