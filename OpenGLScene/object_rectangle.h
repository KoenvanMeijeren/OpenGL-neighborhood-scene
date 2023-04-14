#pragma once
#include "object.h"

class object_rectangle final : public object
{
public:
	object_rectangle(const glm::vec3& position, const glm::vec3& light_position, material* material);
	object_rectangle(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* texture_path);

	void init_shape();
};
