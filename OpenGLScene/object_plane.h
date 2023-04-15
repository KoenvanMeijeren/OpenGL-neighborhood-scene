#pragma once
#include "object.h"

class object_plane final : public object
{
public:
	object_plane(const glm::vec3& position, const glm::vec3& light_position, material* material);
	object_plane(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* texture_path);

	void init_shape();
};
