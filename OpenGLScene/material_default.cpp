#include "material_default.h"

#include "shader_default.h"

material_default::material_default(const glm::vec3& ambient_color, const glm::vec3& diffuse_color,
                                   const glm::vec3& specular_color, const float power)
: material(new shader_default())
{
    data_.ambient_color = ambient_color;
    data_.diffuse_color = diffuse_color;
    data_.specular_color = specular_color;
    data_.power = power;
}
