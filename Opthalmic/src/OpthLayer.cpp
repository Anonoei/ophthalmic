#include "OpthLayer.h"
#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DAGGer/Utils/PlatformUtils.h"

#include "DAGGer/Math/Math.h"

OpthLayer::OpthLayer()
	: Layer("OpthLayer"), m_CameraController(1280.0f / 720.0f)
{
}

void OpthLayer::OnAttach()
{
	

	DAGGer::FramebufferSpecification fbSpec;
	fbSpec.Attachments = { DAGGer::FramebufferTextureFormat::RGBA8, DAGGer::FramebufferTextureFormat::RED_INTEGER, DAGGer::FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = DAGGer::Framebuffer::Create(fbSpec);

	m_ActiveScene = DAGGer::Ref<DAGGer::Scene>::Create();

	m_PlaybackPanel.OnAttach();

	auto note1 = m_ActiveScene->CreateEntity("note1");
	note1.AddComponent<DAGGer::SpriteRendererComponent>(DrColor(1.0f, 0.8f, 0.8f, 1.0f));

	m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
	m_CameraEntity.AddComponent<DAGGer::CameraComponent>().Primary = true;

}

void OpthLayer::OnDetach()
{

}

void OpthLayer::OnUpdate(DAGGer::Timestep ts)
{
	// Resize
	if (DAGGer::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	//	Update
	if (m_ViewportFocused)
		m_CameraController.OnUpdate(ts);

	//	Render
	m_Framebuffer->Bind();
	DAGGer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	DAGGer::RenderCommand::Clear();

	//	Clear entity ID attachment
	m_Framebuffer->ClearAttachment(1, -1);

	//	Update Scene
	m_ActiveScene->OnUpdateRuntime(ts);

	m_Framebuffer->Unbind();
}

void OpthLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Opthalmic", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Opthalmic");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
			if (ImGui::MenuItem("New", "Ctrl+N"))
				NewScene();

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				OpenScene();

			ImGui::Separator();

			if (ImGui::MenuItem("Library"));

			if (ImGui::MenuItem("Exit"))
				DAGGer::Application::Get().Close();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Play"))
		{
			if (ImGui::MenuItem("Play/Pause"));

			if (ImGui::MenuItem("Stop"));

			if (ImGui::MenuItem("Skip Forward"));

			if (ImGui::MenuItem("Skip Backward"));

			ImGui::Separator();

			if (ImGui::MenuItem("Loop"));

			if (ImGui::MenuItem("Faster Playback"));

			if (ImGui::MenuItem("Slower Playback"));

			if (ImGui::MenuItem("Reset"));

			ImGui::Separator();

			if (ImGui::MenuItem("Faster Notes"));

			if (ImGui::MenuItem("Slower Notes"));

			ImGui::Separator();

			if (ImGui::MenuItem("Volume Up"));

			if (ImGui::MenuItem("Volume Down"));

			if (ImGui::MenuItem("Mute"));
				
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Playback Toolbar"));

			if (ImGui::MenuItem("Library Panel"));

			if (ImGui::MenuItem("Keyboard"));

			if (ImGui::MenuItem("Note Labels"));

			ImGui::Separator();

			if (ImGui::MenuItem("Always On Top"));

			ImGui::Separator();

			if (ImGui::MenuItem("Fullscreen"));

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Preferences"));

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	m_LibraryPanel.OnImGuiRender();
	m_PlaybackPanel.OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	DAGGer::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void OpthLayer::UpdateWindowTitle(const std::string& title)
{

}

void OpthLayer::OnEvent(DAGGer::Event& e)
{
	DAGGer::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<DAGGer::KeyPressedEvent>(Dr_BIND_EVENT_FN(OpthLayer::OnKeyPressed));
	dispatcher.Dispatch<DAGGer::MouseButtonPressedEvent>(Dr_BIND_EVENT_FN(OpthLayer::OnMouseButtonPressed));
}

bool OpthLayer::OnMouseButtonPressed(DAGGer::MouseButtonPressedEvent& e)
{
	return false;
}

bool OpthLayer::OnKeyPressed(DAGGer::KeyPressedEvent& e)
{
	if (e.GetRepeatCount() > 0)
		return false;

	bool control = (DAGGer::Input::IsKeyPressed(DAGGer::Key::LeftControl) || DAGGer::Input::IsKeyPressed(DAGGer::Key::RightControl));
	bool shift = (DAGGer::Input::IsKeyPressed(DAGGer::Key::LeftShift) || DAGGer::Input::IsKeyPressed(DAGGer::Key::RightShift));
	switch (e.GetKeyCode())
	{
		case DAGGer::Key::N:	//	Ctrl+N
		{
			if (control)
				NewScene();
			break;
		}
		case DAGGer::Key::O:	//	Ctrl+O
		{
			if (control)
				OpenScene();
			break;
		}
		//case DAGGer::Key::S:	//	Ctrl+S
		//{
		//	if (control && shift)
		//		SaveSceneAs();
		//	else if (control)
		//		SaveScene();
		//	break;
		//}
	}
	return false;
}

void OpthLayer::NewScene()
{
	m_ActiveScene = DAGGer::Ref<DAGGer::Scene>::Create();
	m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
}

void OpthLayer::OpenScene()
{
	std::string filepath = DAGGer::FileDialogs::OpenFile("MIDI file (*.mid)\0*.mid\0");
	if (!filepath.empty())
	{
		m_ActiveScene = DAGGer::Ref<DAGGer::Scene>::Create();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		//DAGGer::SceneSerializer serializer(m_ActiveScene);
		//serializer.Deserialize(filepath);
	}
}