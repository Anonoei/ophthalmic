#pragma once
#include <DAGGer.h>					//	Include DAGGer
#include <DAGGer/Core/Entry.h>		//	Entry Point

#include "OpLayer.h"

namespace DAGGer
{
	class Ophthalmic : public Application
	{
	public:
		Ophthalmic(const DAGGer::ApplicationSpecification& specification, ApplicationCommandLineArgs args)
			: Application(specification)
		{
			Dr_INFO("Ophthalmic");
			PushLayer(new OpLayer());
		}

		~Ophthalmic() {}
	};	//	END class Opthalmic

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		DAGGer::ApplicationSpecification specification;
		specification.Name = "Ophthalmic";
		specification.WindowWidth = 1600;
		specification.WindowHeight = 900;
		specification.StartMaximized = false;
		specification.VSync = true;
		specification.Decorations = true;
		specification.EnableImGui = true;


		return new Ophthalmic(specification, args);
	}	//	END CreateApplication

}	//	END namespace DAGGer