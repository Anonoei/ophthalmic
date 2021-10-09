#include "Note.h"
#include <DAGGer.h>

void Note::Init(float xPos)
{
	m_Instance.AddComponent<DAGGer::SpriteRendererComponent>(DrColor(0.8f, 0.0f, 0.7f, 1.0f));
	auto& transform = m_Instance.GetComponent<DAGGer::TransformComponent>();
	transform.Scale.x = this->Width;
	transform.Scale.y = this->Length;
	transform.Translation.x = xPos;
	transform.Translation.y = 0;

	return;
}

void Note::OnUpdate(DAGGer::Timestep ts)
{
	auto& transform = m_Instance.GetComponent<DAGGer::TransformComponent>();
	transform.Translation.y -= Speed * ts;
}