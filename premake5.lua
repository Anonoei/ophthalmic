workspace "Ophthalmic"
	startproject "Ophthalmic"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	platforms
	{
		"Win64",
		"Win32",
		"Linux64",
		"Linux32",
		"Mac64",
		"Mac32",
		
		"iOS",
		"Anrdoid"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter {"platforms:Win64 or Linux64 or Mac64"}		--	x86_64
		architecture "x86_64"

	filter {"platforms:Win32 or Linux32 or Mac32"}		--	x86
		architecture "x86"

	filter {"platforms:iOS-64 or Android-64"}				--	ARM
		architecture "ARM"

	filter {"system:windows"}			--	WINDOWS
		removeplatforms { "Linux32", "Linux64", "Mac32", "Mac64" }
		defaultplatform "Win64"

	filter {"system:linux"}				--	LINUX
		removeplatforms { "Win32", "Win64", "Mac32", "Mac64" }
		defaultplatform "Linux64"

	filter {"system:macosx"}			--	MACOS
		removeplatforms { "Linux32", "Linux64", "Win32", "Win64" }
		defaultplatform "Mac64"
	filter ""

	-- Debug/Release/Dist /-/ Build on OS /-/ Build for OS
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}"

include "DAGGer"

include "Ophthalmic"