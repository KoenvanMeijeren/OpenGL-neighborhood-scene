#pragma once
#include <vector>
#include "entity.h"

class object_builder
{
protected:
	std::vector<entity *> objects_;
public:
	virtual ~object_builder();
	virtual void init() = 0;
	virtual void render();
};
