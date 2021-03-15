#pragma once

#include "DAGGer/Core/Base.h"
#include <string>

class LibraryPanel
{
public:
	LibraryPanel() = default;
	
	void OnImGuiRender();

	void DrawLibrary();
private:
	std::string m_SelectionContext;
};	//	END class LibraryPanel