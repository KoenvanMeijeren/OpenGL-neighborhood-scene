#pragma once
#include "material.h"

class material_lambert final: public material
{
public:
	material_lambert(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color);
};
