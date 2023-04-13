#include "shader.h"

#include "glsl.h"

shader::shader() = default;

shader::shader(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
	const char* vertexshader = glsl::readFile(vertex_shader_filename);
	const GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	const char* fragshader = glsl::readFile(fragment_shader_filename);
	const GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id_ = glsl::makeShaderProgram(vsh_id, fsh_id);
}

void shader::enable() const
{
	glUseProgram(program_id_);
}
