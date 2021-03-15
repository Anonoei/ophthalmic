#pragma once
#include "DAGGer/Core/Base.h"

#include "DAGGer/Renderer/Texture.h"
#include "DAGGer/Renderer/SubTexture2D.h"

class PlaybackPanel
{
public:
	PlaybackPanel() = default;

	void OnAttach();

	void OnImGuiRender();
private:
	DAGGer::Ref<DAGGer::Texture2D> m_Icons;
	DAGGer::Ref<DAGGer::SubTexture2D> m_PlayButton;
	DAGGer::Ref<DAGGer::SubTexture2D> m_PauseButton;
	DAGGer::Ref<DAGGer::SubTexture2D> m_StopButton;
	DAGGer::Ref<DAGGer::SubTexture2D> m_ReverseButton;
	DAGGer::Ref<DAGGer::SubTexture2D> m_ForwardButton;
	DAGGer::Ref<DAGGer::SubTexture2D> m_SpeakerIcon;
};

