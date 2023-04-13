#include "shader_factory.h"

shader_manager create_texture_shader()
{
	return {"vertexshader_texture.vert", "fragmentshader_texture.frag"};
}
