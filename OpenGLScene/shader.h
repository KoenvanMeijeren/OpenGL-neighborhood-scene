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
public:
	shader(const char* vertex_shader_filename, const char* fragment_shader_filename);
	virtual ~shader();

	virtual void init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);

	// Needs to be called every time we want to do something with the program id.
	// Because there can only be one active shader at the time used by the program.
	virtual void enable();

	// Fill uniform vars (needed in fragment shaders (abstract method)
	virtual void fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material) = 0;

	// Update the vao and send it
	virtual void send_vao(const std::vector<glm::vec3>& vertices);

	// Write the updated values back to the shader (abstract method)
	virtual void update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const material& material, const std::vector<glm::vec3>& vertices) = 0;
};
