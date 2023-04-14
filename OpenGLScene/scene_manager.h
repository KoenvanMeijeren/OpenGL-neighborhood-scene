#pragma once
#include <vector>
#include "entity.h"

class scene_manager
{
private:
	camera *camera_;

	std::vector<entity *> objects_;
public:
	explicit scene_manager();
	~scene_manager();

	// Creates the objects and fills the needed variables.
	void init();

	// Renders the world.
	void render() const;
};
