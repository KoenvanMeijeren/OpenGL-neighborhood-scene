#include "matrix_transformations.h"

glm::mat4 matrix_scale(const glm::mat4& model, const float x, const float y, const float z)
{
    return glm::scale(model, glm::vec3(x, y, z));
}

glm::mat4 matrix_scale(const glm::mat4& model, const float scale)
{
    return glm::scale(model, glm::vec3(scale, scale, scale));
}

glm::mat4 matrix_rotate(const glm::mat4& model, const float rotate_speed, const float x, const float y, const float z)
{
     return glm::rotate(model, rotate_speed, glm::vec3(x, y, z));
}

glm::mat4 matrix_rotate(const glm::mat4& model, const float rotate_speed, const float rotate_value)
{
    return glm::rotate(model, rotate_speed, glm::vec3(rotate_value, rotate_value, rotate_value));
}

glm::mat4 matrix_translate(const glm::mat4& model, const float x, const float y, const float z)
{
    return glm::translate(model, glm::vec3(x, y, z));
}

glm::mat4 matrix_translate(const glm::mat4& model, const float translate_value)
{
    return glm::translate(model, glm::vec3(translate_value, translate_value, translate_value));
}
