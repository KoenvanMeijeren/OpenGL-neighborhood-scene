#include "material.h"

material::material(shader* shader)
{
	shader_ = shader;
}

material::~material()
{
	delete shader_;
}

void material::set_texture_id(const GLuint texture_id) const
{
	shader_->set_texture_id(texture_id);
}

void material::init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals,
                            const std::vector<glm::vec2>& uvs) const
{
	shader_->init_buffers(vertices, normals, uvs);
    shader_->enable();
}

void material::render(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light,
	const std::vector<glm::vec3>& vertices) const
{
	shader_->update(model_view, projection, light, data_, vertices);
}
