#pragma once
#include "shader.h"

class shader_line_shape
{
public:
	// ID's
	GLuint program_id{};
	GLuint vao{};
	
	// Uniform ID's
	GLint uniform_mvp{};

	shader_line_shape();
	shader_line_shape(const char* vertex_shader_filename, const char* fragment_shader_filename);
	~shader_line_shape() = default;

	void init_buffers(const GLfloat vertices[], const GLsizeiptr vertices_size, const GLfloat colors[], const GLsizeiptr colors_size, const GLushort cube_elements[], const GLsizeiptr cube_elements_size);

	// Needs to be called every time we want to do something with the program id.
	// Because there can only be one active shader at the time used by the program.
	void enable() const;

	// Fill uniform vars (needed in fragment shaders (abstract method)
	void fill_uniform_vars(const glm::mat4& mvp) const;

	// Update the vao and send it
	void send_vao(const GLushort cube_elements[], const GLsizei cube_elements_count) const;

	// Write the updated values back to the shader (abstract method)
	void update(const glm::mat4& mvp, const GLushort cube_elements[], const GLsizei cube_elements_count) const;
};
