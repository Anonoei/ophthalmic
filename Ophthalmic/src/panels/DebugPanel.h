#pragma once

#include "objects/Manager.h"

#include <imgui/imgui.h>

class DebugPanel
{
public:
	DebugPanel() {}

	void Init(Manager* manager);

	void OnImGuiRender();
private:
	Manager* m_Manager = nullptr;
};

