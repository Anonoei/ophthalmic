#pragma once

#include "DAGGer/Renderer/Texture.h"
#include "DAGGer/Events/Event.h"
#include "DAGGer/Events/KeyEvent.h"

#include <filesystem>
#include <vector>

#include <imgui/imgui.h>

class LibraryPanel
{
public:
	LibraryPanel();

	void OnImGuiRender();
private:
	std::vector<std::filesystem::path> m_Paths;

	DAGGer::Ref<DAGGer::Texture2D> m_FileIcon;
};

