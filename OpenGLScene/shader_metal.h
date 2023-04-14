#pragma once
#include "shader.h"

class shader_metal final: public shader
{
private:
	GLuint texture_id_{};

	// Uniform ID's
	GLint uniform_material_ambient_{};
	GLint uniform_material_diffuse_{};
	GLint uniform_material_specular_{};
	GLint uniform_material_power_{};
	GLint uniform_material_metalness_{};
	GLint uniform_material_roughness_{};
	GLint uniform_model_view_{};
	GLint uniform_projection_{};
	GLint uniform_light_position_{};
	GLint uniform_apply_texture_{};
	bool has_texture_ = false;
public:
	shader_metal();

	void set_texture_id(const GLuint texture_id) override;
	void init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) override;
	void fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material_struct& material) override;
	void update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material_struct& material, const std::vector<glm::vec3>& vertices) override;
};
