#pragma once
#include "object.h"

class object_cube final : public object
{
public:
	object_cube(const glm::vec3& position, const glm::vec3& light_position, material* material);
	object_cube(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* texture_path);

	void init_shape();
};
