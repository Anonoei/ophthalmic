#include "LibraryPanel.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

void LibraryPanel::OnImGuiRender()
{
	ImGui::Begin("Library Panel");

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_SelectionContext = {};

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Add MIDI file"));
		if (ImGui::MenuItem("Add folder"));

		ImGui::EndPopup();
	}

	DrawLibrary();

	ImGui::End();
}

void LibraryPanel::DrawLibrary()
{
	const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)111111, flags, "Mock file");
	if (ImGui::IsItemClicked())
		m_SelectionContext = "Mock file";

	bool itemDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Item"))
			itemDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGui::TreePop();
	}

	if (itemDeleted)
	{

	}
}