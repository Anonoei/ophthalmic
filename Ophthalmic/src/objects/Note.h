#pragma once

#include "DAGGer/Core/Timestep.h"
#include "DAGGer/Scene/Entity.h"

#include <stdint.h>

/*
	A note needs to know it's location
	Location is related to the size of the keyboard, specifially each key
	This let's us know how wide the not should be, and where to draw it
*/

class Note
{
public:
	Note(DAGGer::Entity note, size_t index, float length, float width, float xPos, const char* label = "")
		: m_Instance(note), Index(index), Length(length), Width(width), Label(label)
	{
		Init(xPos);
	}
	~Note() {}

	void Init(float xPos);

	void OnUpdate(DAGGer::Timestep ts);
private:
	size_t Index;

	DAGGer::Entity m_Instance;
	const char* Label;
	float Length;
	float Width;

	float Speed = 0.5f;
};

