#pragma once
#include "material.h"

class material_metal final: public material
{
public:
	material_metal(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, const float power, const float metalness = 0, const float roughness = 0);
};
