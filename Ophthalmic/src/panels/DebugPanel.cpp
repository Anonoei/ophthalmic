#include "DebugPanel.h"

void DebugPanel::Init(Manager* manager)
{
	m_Manager = manager;
}

void DebugPanel::OnImGuiRender()
{
	ImGui::Begin("Debug");
	ImGui::Text("Manager Stats");
	ImGui::Separator();
	ImGui::Text("Notes: %i", m_Manager->GetSize());
	ImGui::End();
}