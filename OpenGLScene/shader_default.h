﻿#pragma once
#include "shader.h"

class shader_default final: public shader
{
private:
	// Uniform ID's
	GLint uniform_material_ambient_{};
	GLint uniform_material_diffuse_{};
	GLint uniform_material_specular_{};
	GLint uniform_material_power_{};
	GLint uniform_model_view_{};
	GLint uniform_projection_{};
	GLint uniform_light_position_{};
public:
	shader_default();

	void init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) override;
	void fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material) override;

	void update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material, const std::vector<glm::vec3>& vertices) override;
};
