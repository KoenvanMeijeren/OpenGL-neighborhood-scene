#pragma once
#include <GL/glew.h>

class shader
{
public:
	GLuint program_id_{};
	shader();
	shader(const char* vertex_shader_filename, const char* fragment_shader_filename);

	void enable() const;
};
