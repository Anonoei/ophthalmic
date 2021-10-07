#include "LibraryPanel.h"

LibraryPanel::LibraryPanel()
{
	m_FileIcon = DAGGer::Texture2D::Create("Resources/Icons/Library/FileIcon.png");
}

void LibraryPanel::OnImGuiRender()
{
	ImGui::Begin("Library");
	ImGui::Text("MIDI files will show up here");
	ImGui::End();
}