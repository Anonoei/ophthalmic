#pragma once
/*
	This class manages notes and the keyboard
	Since notes need access to the keyboard, this sends updates
*/

#include "Keyboard.h"
#include "Note.h"

#include <vector>
#include "DAGGer/Core/Timestep.h"
#include "DAGGer/Scene/Scene.h"
#include "DAGGer/Scene/Entity.h"

class Manager
{
public:
	Manager() {}
	~Manager() {}

	void CreateNote(DAGGer::Ref<DAGGer::Scene> scene);

	void OnUpdate(DAGGer::Timestep ts);

	//	Getters
	size_t GetSize() { return m_Notes.size(); }
private:
	Keyboard m_Keyboard;
	std::vector<Note> m_Notes;
};