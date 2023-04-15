#pragma once
#include "object-builder.h"

class primitive_person_builder final: public object_builder
{
public:
	void init() override;
};