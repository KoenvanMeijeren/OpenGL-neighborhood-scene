#pragma once

#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 matrix_scale(const glm::mat4& model, const float x, const float y, const float z);
glm::mat4 matrix_scale(const glm::mat4& model, const float scale);

glm::mat4 matrix_rotate(const glm::mat4& model, const float rotate_speed, const float x, const float y, const float z);
glm::mat4 matrix_rotate(const glm::mat4& model, const float rotate_speed, const float rotate_value);

glm::mat4 matrix_translate(const glm::mat4& model, const float x, const float y, const float z);
glm::mat4 matrix_translate(const glm::mat4& model, const float translate_value);