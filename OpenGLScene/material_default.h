#pragma once
#include "material.h"

class material_default final: public material
{
public:
	material_default(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, const float power);
};
