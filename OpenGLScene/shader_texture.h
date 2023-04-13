#pragma once
#include "shader.h"

class shader_texture final: public shader
{
public:
	explicit shader_texture(const GLuint texture_id);
};
