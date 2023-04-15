#pragma once
#include "object.h"

class object_triangle final : public object
{
public:
	object_triangle(const glm::vec3& position, const glm::vec3& light_position, material* material);
	object_triangle(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* texture_path);

	void init_shape();
};
