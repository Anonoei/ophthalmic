#pragma once

#include <DAGGer.h>

#include "Opthalmic/src/panels/LibraryPanel.h"
#include "Opthalmic/src/panels/PlaybackPanel.h"

#include "DAGGer/Renderer/OrthographicCamera.h"

class OpthLayer : public DAGGer::Layer
{
public:
	OpthLayer();
	virtual ~OpthLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(DAGGer::Timestep ts) override;

	virtual void OnImGuiRender() override;
	void OnEvent(DAGGer::Event& e) override;
private:
	void UpdateWindowTitle(const std::string& sceneName);

	bool OnKeyPressed(DAGGer::KeyPressedEvent& e);
	bool OnMouseButtonPressed(DAGGer::MouseButtonPressedEvent& e);
	void NewScene();
	void OpenScene();
private:
	DAGGer::Ref<DAGGer::Framebuffer> m_Framebuffer;
	DAGGer::Ref<DAGGer::Scene> m_ActiveScene;
	DAGGer::Entity m_CameraEntity;

	DAGGer::OrthographicCameraController m_CameraController;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = DrPos2(0.0f, 0.0f);
	glm::vec2 m_ViewportBounds[2];

	//	Panels
	LibraryPanel m_LibraryPanel;
	PlaybackPanel m_PlaybackPanel;
};