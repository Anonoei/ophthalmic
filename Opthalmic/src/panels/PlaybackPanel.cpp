#include "PlaybackPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

void PlaybackPanel::OnAttach()
{
	//	Load assets
	m_Icons = DAGGer::Texture2D::Create("assets/textures/Icons.png");
	m_PlayButton = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(0, 3), DrSize2(256.0f, 256.0f));
	m_PauseButton = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(1, 3), DrSize2(256.0f, 256.0f));
	m_StopButton = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(2, 3), DrSize2(256.0f, 256.0f));

	m_ForwardButton = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(0, 3), DrSize2(256.0f, 256.0f));
	m_ReverseButton = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(1, 3), DrSize2(256.0f, 256.0f));

	m_SpeakerIcon = DAGGer::SubTexture2D::CreateFromCoords(m_Icons, DrSize2(0, 2), DrSize2(256.0f, 256.0f));
}
void PlaybackPanel::OnImGuiRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

	ImGui::Begin("Playback Toolbar");

	ImGui::Columns(4);

	//	PlayButton
	if (ImGui::ImageButton((ImTextureID)(m_PlayButton->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{

	}
	ImGui::SameLine();
	//	PauseButton
	if (ImGui::ImageButton((ImTextureID)(m_PauseButton->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{

	}
	ImGui::SameLine();
	//	StopButton
	if (ImGui::ImageButton((ImTextureID)(m_StopButton->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{

	}
	ImGui::SameLine();

	ImGui::NextColumn();

	//	ForwardButton
	if (ImGui::ImageButton((ImTextureID)(m_ForwardButton->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{

	}
	ImGui::SameLine();
	//	ReverseButton
	if (ImGui::ImageButton((ImTextureID)(m_ReverseButton->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{

	}
	ImGui::SameLine();

	ImGui::NextColumn();

	//	PlayButton
	if (ImGui::ImageButton((ImTextureID)(m_SpeakerIcon->GetRendererID()), ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.9f, 0.9f, 0.9f, 1.0f)))
	{
		ImGui::SameLine();
		int volume = 0.0f;
		ImGui::DragInt("", &volume, 1.0);
	}

	ImGui::NextColumn();

	//	Playback
	ImGui::SameLine();
	ImGui::Text("Playback:");
	ImGui::SameLine();
	float speed = 0.0f;
	ImGui::DragFloat("", &speed, 1.0f);

	//	Note length
	ImGui::SameLine();
	ImGui::Text("Notes:");
	ImGui::SameLine();
	float notelength = 0.0f;
	ImGui::DragFloat("", &notelength, 1.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}