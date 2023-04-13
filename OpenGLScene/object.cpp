#include "object.h"

#include <GL/gl.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "matrix_transformations.h"
#include "objloader.h"
#include "shader_default.h"
#include "shader_texture.h"
#include "texture.h"

object::object(const float x, const float y, const float z)
{
	camera_ = camera::get_instance();
	model_ = glm::translate(glm::mat4(), glm::vec3(x, y, z));
	model_view_ = model_ * camera_->get_view();
    shader_ = shader_default();
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

void object::set_object(const char* object_path)
{
	loadOBJ(object_path, vertices_, uvs_, normals_);
}

void object::set_texture(const char* texture_image_path)
{
	const GLuint texture_id = loadBMP(texture_image_path);
    shader_ = shader_texture(texture_id);
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
    shader_.init_buffers(vertices_, normals_, uvs_);
    shader_.enable();
}

void object::render()
{
    // Before doing anything, update the camera.
    camera_->update();

    // Perform the configured animations.
    for (const auto animation : animations_)
    {
	    model_ = animation->execute(model_);
    }

    // Note that this calculation is different from the initial calculation.
    // This is done because it prevents the model to fly away out of the screen.
    model_view_ = camera_->get_view() * model_;

    // Before ending the rendering of the object, the shader needs to be updated.
    shader_.update(model_view_, camera_->get_projection(), light_, material_, vertices_);
}
