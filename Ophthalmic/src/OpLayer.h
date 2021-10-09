#pragma once

#include <DAGGer.h>
#include "DAGGer/Renderer/EditorCamera.h"

#include "panels/LibraryPanel.h"
#include "panels/DebugPanel.h"

#include "objects/Manager.h"

#define OPHTHALMIC_BUILD_ID "v0.0.1"

class OpLayer : public DAGGer::Layer
{
public:
	OpLayer();
	virtual ~OpLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(DAGGer::Timestep ts) override;

	virtual void OnImGuiRender() override;
	void OnEvent(DAGGer::Event& e) override;

	//	UI Panels
	void UI_Toolbar();
private:
	void UpdateWindowTitle();

	bool OnKeyPressed(DAGGer::KeyPressedEvent& e);
	bool OnMouseButtonPressed(DAGGer::MouseButtonPressedEvent& e);
	void NewScene();
	void OpenScene();
private:
	DAGGer::Ref<DAGGer::Framebuffer> m_Framebuffer;
	DAGGer::Ref<DAGGer::Scene> m_ActiveScene;
	DAGGer::Entity m_CameraEntity;
	DAGGer::OrthographicCameraController m_CameraController;

	DAGGer::EditorCamera m_EditorCamera;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = DrPos2(0.0f, 0.0f);
	glm::vec2 m_ViewportBounds[2];

	Manager m_Manager;

	//	Textures
	DAGGer::Ref<DAGGer::Texture2D> m_IconPlay, m_IconPause, m_IconStop;

	//	Panels
	LibraryPanel m_LibraryPanel;
	DebugPanel m_DebugPanel;
};