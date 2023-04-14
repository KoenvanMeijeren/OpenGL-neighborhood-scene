#include "object_primitive.h"

#include "matrix_transformations.h"

object_primitive::object_primitive(const float x, const float y, const float z)
{
    camera_ = camera::get_instance();
	shader_ = new shader_primitive();
    model_ = matrix_translate(glm::mat4(), x, y, z);
    model_view_projection_ = camera_->get_projection() * camera_->get_view() * model_;
    init_buffers();
}

object_primitive::~object_primitive()
{
    delete camera_;
    delete shader_;
}

void object_primitive::add_animation(animation* animation)
{
    animations_.push_back(animation);
}

void object_primitive::scale(const float x, const float y, const float z)
{
    model_ = matrix_scale(model_, x, y, z);
}

void object_primitive::scale(const float scale)
{
    model_ = matrix_scale(model_, scale);
}

void object_primitive::rotate(const float rotate_speed, const float x, const float y, const float z)
{
     model_ = matrix_rotate(model_, rotate_speed, x, y, z);
}

void object_primitive::rotate(const float rotate_speed, const float rotate_value)
{
    model_ = matrix_rotate(model_, rotate_speed, rotate_value);
}

void object_primitive::translate(const float x, const float y, const float z)
{
    model_ = matrix_translate(model_, x, y, z);
}

void object_primitive::translate(const float translate_value)
{
    model_ = matrix_translate(model_, translate_value);
}

void object_primitive::init_buffers() const
{
	shader_->init_buffers(
        vertices_, sizeof(vertices_), 
        colors_, sizeof(colors_),
        cube_elements_, sizeof(cube_elements_)
    );
    shader_->enable();
    shader_->fill_uniform_vars(model_view_projection_);
}

void object_primitive::render()
{
    // Before doing anything, update the camera.
    camera_->update();

    // Perform the configured animations.
    for (const auto animation : animations_)
    {
	    model_ = animation->execute(model_);
    }

    // Update model view projection
    model_view_projection_ = camera_->get_projection() * camera_->get_view() * model_;

    // Before ending the rendering of the object, the shader needs to be updated.
    shader_->update(model_view_projection_, cube_elements_, sizeof(cube_elements_) / sizeof(GLushort));
}
