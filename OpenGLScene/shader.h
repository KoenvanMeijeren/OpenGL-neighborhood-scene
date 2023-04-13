#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "configuration.h"

class shader
{
protected:
	GLuint program_id_{};
	GLuint vao_{};
	GLuint texture_id_{};

	// Uniform ID's
	GLint uniform_material_ambient_{};
	GLint uniform_material_diffuse_{};
	GLint uniform_material_specular_{};
	GLint uniform_material_power_{};
	GLint uniform_apply_texture_{};
	GLint uniform_mv_{};
	GLint uniform_projection_{};
	GLint uniform_light_pos_{};
public:
	shader();
	shader(const char* vertex_shader_filename, const char* fragment_shader_filename);
	virtual ~shader();

	virtual void init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
	virtual void enable();

	// Fill uniform vars (needed in fragment shaders
	virtual void fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material);
	virtual void send_vao(const std::vector<glm::vec3>& vertices);

	// Write the updated values back to the shader
	void update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material, const std::vector<glm::vec3>& vertices);
};
