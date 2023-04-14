#include "object_primitive.h"

#include "matrix_transformations.h"

object_primitive::object_primitive()
{
    camera_ = camera::get_instance();
	shader_ = new shader_primitive();
    model_ = matrix_translate(glm::mat4(), 3, 3, 0);
    mvp_ = camera_->get_projection() * camera_->get_view() * model_;
    init_buffers();
}

void object_primitive::init_buffers() const
{
	shader_->init_buffers(
        vertices_, sizeof(vertices_), 
        colors_, sizeof(colors_),
        cube_elements_, sizeof(cube_elements_)
    );
    shader_->enable();
    shader_->fill_uniform_vars(mvp_);
}

void object_primitive::render()
{
    // Before doing anything, update the camera.
    camera_->update();

    // Perform the configured animations.
	model_ = glm::rotate(model_, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp_ = camera_->get_projection() * camera_->get_view() * model_;

    // Before ending the rendering of the object, the shader needs to be updated.
    shader_->update(mvp_, cube_elements_, sizeof(cube_elements_) / sizeof(GLushort));
}
