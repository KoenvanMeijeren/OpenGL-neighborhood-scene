#include "shader_factory.h"

shader create_default_shader()
{
	return {"vertexshader_default.vert", "fragmentshader_default.frag"};
}

shader create_texture_shader()
{
	return {"vertexshader_default.vert", "fragmentshader_texture.frag"};
}
