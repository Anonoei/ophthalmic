#include "opthalmic.h"

Opthalmic::Opthalmic()
	: Application("Opthalmic")
{
	opthLayer = new OpthLayer();
	PushLayer(opthLayer);
}

Opthalmic::~Opthalmic()
{
}

DAGGer::Application* DAGGer::CreateApplication()
{
	return new Opthalmic();
}