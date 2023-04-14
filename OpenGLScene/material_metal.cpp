#include "material_metal.h"

#include "shader_metal.h"

material_metal::material_metal(const glm::vec3& ambient_color, const glm::vec3& diffuse_color,
                                 const glm::vec3& specular_color, const float power, const float metalness, const float roughness)
: material(new shader_metal())
{
    data_.ambient_color = ambient_color;
    data_.diffuse_color = diffuse_color;
    data_.specular_color = specular_color;
    data_.power = power;
    data_.metalness = metalness;
    data_.roughness = roughness;
}
