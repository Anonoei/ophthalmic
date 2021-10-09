#include "Manager.h"

void Manager::CreateNote(DAGGer::Ref<DAGGer::Scene> scene)
{
	auto NoteEntity = scene->CreateEntity("note");
	Note note = Note(NoteEntity, m_Notes.size(), 0.4f, 0.125f, 0);
	m_Notes.push_back(note);
}

void Manager::OnUpdate(DAGGer::Timestep ts)
{
	for (auto note : m_Notes)
		note.OnUpdate(ts);
}