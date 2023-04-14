#pragma once
#include <vector>
#include "object.h"
#include "line_shape.h"

class scene_manager
{
private:
	camera *camera_;

	std::vector<object *> objects_;
	std::vector<line_shape *> line_shape_objects_;
public:
	explicit scene_manager();
	~scene_manager();

	// Creates the objects and fills the needed variables.
	void init();

	// Renders the world.
	void render() const;
};
