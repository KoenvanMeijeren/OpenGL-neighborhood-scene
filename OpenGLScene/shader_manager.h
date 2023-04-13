#pragma once
#include <GL/glew.h>

class shader_manager
{
public:
	GLuint program_id{};

	shader_manager();
	shader_manager(const char* vertex_shader_filename, const char* fragment_shader_filename);

	void enable() const;
};
