#include "object.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "matrix_transformations.h"
#include "objloader.h"
#include "shader_factory.h"
#include "texture.h"

object::object(const float x, const float y, const float z)
{
	camera_ = camera::get_instance();
	model_ = glm::translate(glm::mat4(), glm::vec3(x, y, z));
	mv_ = model_ * camera_->get_view();
    set_shader(create_default_shader());
}

object::~object()
{
    delete camera_;
    for (const auto animation : animations_)
    {
	    delete animation;
    }
}

glm::mat4 object::model()
{
    return model_;
}

void object::set_shader(const shader shader)
{
    shader_ = shader;
}

void object::set_object(const char* object_path)
{
	loadOBJ(object_path, vertices_, uvs_, normals_);
}

void object::set_texture(const char* texture_image_path)
{
	texture_id_ = loadBMP(texture_image_path);
	apply_texture_ = true;
    set_shader(create_texture_shader());
}

void object::set_light(const glm::vec3& light_position)
{
	light_.position = light_position;
}

void object::set_material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, const float power)
{
	material_.ambient_color = ambient_color;
    material_.diffuse_color = diffuse_color;
    material_.specular_color = specular_color;
    material_.power = power;
}

void object::add_animation(animation* animation)
{
    animations_.push_back(animation);
}

void object::scale(const float x, const float y, const float z)
{
    model_ = matrix_scale(model_, x, y, z);
}

void object::scale(const float scale)
{
    model_ = matrix_scale(model_, scale);
}

void object::rotate(const float rotate_speed, const float x, const float y, const float z)
{
     model_ = matrix_rotate(model_, rotate_speed, x, y, z);
}

void object::rotate(const float rotate_speed, const float rotate_value)
{
    model_ = matrix_rotate(model_, rotate_speed, rotate_value);
}

void object::translate(const float x, const float y, const float z)
{
    model_ = matrix_translate(model_, x, y, z);
}

void object::translate(const float translate_value)
{
    model_ = matrix_translate(model_, translate_value);
}

void object::init_buffers()
{
	GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_uvs;

    // Get vertex attributes
	const GLuint position_id = glGetAttribLocation(shader_.program_id_, "position");
	const GLuint normal_id = glGetAttribLocation(shader_.program_id_, "normal");
	const GLuint uv_id = glGetAttribLocation(shader_.program_id_, "uv");

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for normals
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), normals_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for uv
    glGenBuffers(1, &vbo_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(glm::vec2), uvs_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Allocate memory for vao
    glGenVertexArrays(1, &vao_);

    // Bind to vao
    glBindVertexArray(vao_);

    // Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind normals to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind uv to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
    glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uv_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Stop bind to vao
    glBindVertexArray(0);

    // Make uniform vars
    uniform_mv_ = glGetUniformLocation(shader_.program_id_, "mv");
	uniform_projection_ = glGetUniformLocation(shader_.program_id_, "projection");
	uniform_light_pos_ = glGetUniformLocation(shader_.program_id_, "light_pos");
    uniform_material_ambient_ = glGetUniformLocation(shader_.program_id_, "mat_ambient");
    uniform_material_diffuse_ = glGetUniformLocation(shader_.program_id_, "mat_diffuse");
    uniform_material_specular_ = glGetUniformLocation(shader_.program_id_, "mat_specular");
    uniform_material_power_ = glGetUniformLocation(shader_.program_id_, "mat_power");
    uniform_apply_texture_ = glGetUniformLocation(shader_.program_id_, "apply_texture");

    shader_.enable();
}

void object::render()
{
    shader_.enable();

    // Before doing anything, update the camera.
    camera_->update();
	glUniformMatrix4fv(uniform_projection_, 1, GL_FALSE, glm::value_ptr(camera_->get_projection()));
    glUniform3fv(uniform_light_pos_, 1, glm::value_ptr(light_.position));

    // Perform the configured animations.
    for (const auto animation : animations_)
    {
	    model_ = animation->execute(model_);
    }

    // Update model view and send it
    // Note that this calculation is different from the initial calculation.
    // This is done because it prevents the model to fly away out of the screen.
    mv_ = camera_->get_view() * model_;
    glUniformMatrix4fv(uniform_mv_, 1, GL_FALSE, glm::value_ptr(mv_));

    // Bind textures
    glUniform1i(uniform_apply_texture_, 0);
    if (apply_texture_)
    {
        glUniform1i(uniform_apply_texture_, 1);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }

    // Fill uniform vars (needed in fragment shaders
    glUniform3fv(uniform_material_ambient_, 1, glm::value_ptr(material_.ambient_color));
    glUniform3fv(uniform_material_diffuse_, 1, glm::value_ptr(material_.diffuse_color));
    glUniform3fv(uniform_material_specular_, 1, glm::value_ptr(material_.specular_color));
    glUniform1f(uniform_material_power_, material_.power);

    // Send vao
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    glBindVertexArray(0);
}
