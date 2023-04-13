#include "material_lambert.h"

#include "shader_lambert.h"

material_lambert::material_lambert(const glm::vec3& ambient_color, const glm::vec3& diffuse_color,
                                   const glm::vec3& specular_color): material(new shader_lambert())
{
    data_.ambient_color = ambient_color;
    data_.diffuse_color = diffuse_color;
    data_.specular_color = specular_color;
}
