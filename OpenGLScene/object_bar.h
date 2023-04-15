#pragma once
#include "object.h"

class object_bar final : public object
{
public:
	object_bar(const glm::vec3& position, const glm::vec3& light_position, material* material);
	object_bar(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* texture_path);

	void init_shape();
};