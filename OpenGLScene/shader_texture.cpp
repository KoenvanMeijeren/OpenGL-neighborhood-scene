#include "shader_texture.h"

shader_texture::shader_texture(const GLuint texture_id)
: shader("vertexshader_default.vert", "fragmentshader_texture.frag")
{
	texture_id_ = texture_id;
}
