#pragma once
#include <vector>
#include "object.h"

class scene_manager
{
private:
	camera *camera_;

	std::vector<object> objects_;
public:
	// Creates the objects and fills the needed variables.
	explicit scene_manager();

	// Initializes the object manager and prepares it for rendering.
	void init();

	// Renders the world.
	void render() const;
};
