#include "shape.h"

#include "matrix_transformations.h"

shape::shape(const float x, const float y, const float z)
{
	shader_ = new shader_primitive();
    model_ = matrix_translate(glm::mat4(), x, y, z);
    model_view_projection_ = camera_->get_projection() * camera_->get_view() * model_;
    init_buffers();
}

shape::~shape()
{
    entity::~entity();

    delete shader_;
}

void shape::init_buffers()
{
	shader_->init_buffers(
        vertices_, sizeof(vertices_), 
        colors_, sizeof(colors_),
        cube_elements_, sizeof(cube_elements_)
    );
    shader_->enable();
    shader_->fill_uniform_vars(model_view_projection_);
}

void shape::render()
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
