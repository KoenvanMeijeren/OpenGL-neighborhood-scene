#pragma once
#include <vector>
#include "object.h"
#include "object_primitive.h"

class scene_manager
{
private:
	camera *camera_;

	std::vector<object *> objects_;
	std::vector<object_primitive *> primitive_objects_;
public:
	explicit scene_manager();
	~scene_manager();

	// Creates the objects and fills the needed variables.
	void init();

	// Renders the world.
	void render() const;
};
