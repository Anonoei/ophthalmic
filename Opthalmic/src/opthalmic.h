#pragma once
#include <DAGGer.h>					//	Include DAGGer
#include <DAGGer/Core/Entry.h>		//	Include Entry Point

#include "OpthLayer.h"

class Opthalmic : public DAGGer::Application
{
public:
	Opthalmic();

	~Opthalmic();
private:
	OpthLayer* opthLayer;
};	//	END class Opthalmic