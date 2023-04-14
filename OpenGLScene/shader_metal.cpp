#include "shader_metal.h"

shader_metal::shader_metal()
: shader("vertexshader_default.vert", "fragmentshader_metal.frag")
{
}

void shader_metal::set_texture_id(const GLuint texture_id)
{
    texture_id_ = texture_id;
    has_texture_ = true;
}

void shader_metal::init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals,
                                  const std::vector<glm::vec2>& uvs)
{
	shader::init_buffers(vertices, normals, uvs);

	// Make uniform vars
    uniform_model_view_ = glGetUniformLocation(program_id_, "mv");
	uniform_projection_ = glGetUniformLocation(program_id_, "projection");
	uniform_light_position_ = glGetUniformLocation(program_id_, "light_pos");
    uniform_material_ambient_ = glGetUniformLocation(program_id_, "mat_ambient");
    uniform_material_diffuse_ = glGetUniformLocation(program_id_, "mat_diffuse");
    uniform_material_specular_ = glGetUniformLocation(program_id_, "mat_specular");
    uniform_material_power_ = glGetUniformLocation(program_id_, "mat_power");
    uniform_material_metalness_ = glGetUniformLocation(program_id_, "mat_metalness");
    uniform_material_roughness_ = glGetUniformLocation(program_id_, "mat_roughness");
    uniform_apply_texture_ = glGetUniformLocation(program_id_, "apply_texture");
}

void shader_metal::fill_uniform_vars(const glm::mat4& model_view, const glm::mat4& projection,
	const light_source& light, const material_struct& material)
{
	glUniformMatrix4fv(uniform_model_view_, 1, GL_FALSE, glm::value_ptr(model_view));
    glUniformMatrix4fv(uniform_projection_, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(uniform_light_position_, 1, glm::value_ptr(light.position));
    glUniform3fv(uniform_material_ambient_, 1, glm::value_ptr(material.ambient_color));
    glUniform3fv(uniform_material_diffuse_, 1, glm::value_ptr(material.diffuse_color));
    glUniform3fv(uniform_material_specular_, 1, glm::value_ptr(material.specular_color));
    glUniform1f(uniform_material_power_, material.power);
    glUniform1f(uniform_material_metalness_, material.metalness);
    glUniform1f(uniform_material_roughness_, material.roughness);

    // Bind textures
    glUniform1i(uniform_apply_texture_, 0);
    if (has_texture_)
    {
        glUniform1i(uniform_apply_texture_, 1);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }
}

void shader_metal::update(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light,
	const material_struct& material, const std::vector<glm::vec3>& vertices)
{
    shader::enable();
    shader_metal::fill_uniform_vars(model_view, projection, light, material);
    shader::send_vao(vertices);
}
