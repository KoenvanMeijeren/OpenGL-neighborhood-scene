#include "object.h"

#include <iostream>
#include <GL/gl.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "matrix_transformations.h"
#include "objloader.h"
#include "texture.h"

object::object(const glm::vec3& position, const glm::vec3& light_position, material* material)
{
	model_ = glm::translate(glm::mat4(), position);
	model_view_ = model_ * camera_->get_view();
    light_.position = light_position;
    material_ = material;
}

object::object(const glm::vec3& position, const glm::vec3& light_position, material* material,
	const char* object_path): object(position, light_position, material)
{
	set_object(object_path);
	object::init_buffers();
}

object::object(const glm::vec3& position, const glm::vec3& light_position, material* material,
	const char* object_path, const char* texture_image_path): object(position, light_position, material)
{
	set_object(object_path);
	set_texture(texture_image_path);
	object::init_buffers();
}

object::~object()
{
    entity::~entity();

    delete material_;
}

void object::set_object(const char* object_path)
{
	loadOBJ(object_path, vertices_, uvs_, normals_);
}

void object::set_texture(const char* texture_image_path) const
{
	const GLuint texture_id = loadBMP(texture_image_path);
    material_->set_texture_id(texture_id);
}

void object::init_buffers()
{
    material_->init_buffers(vertices_, normals_, uvs_);
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
    material_->render(model_view_, camera_->get_projection(), light_, vertices_);
}

void object::print_structure() const
{
	std::cout << "Vertices of object: \n";
    print_vertices();

	std::cout << "UVS of object: \n";
    print_uvs();

	std::cout << "Normals of object: \n";
    print_normals();
}

void object::print_vertices() const
{
	for (const auto& vertex : vertices_)
	{
		std::cout << "{" << vertex.x << ", " << vertex.y << ", " << vertex.z << "},\n";
	}
}

void object::print_normals() const
{
    for (const auto& normal : normals_)
	{
		std::cout << "{" << normal.x << ", " << normal.y << ", " << normal.z << "},\n";
	}
}

void object::print_uvs() const
{
    for (const auto& uv : uvs_)
	{
		std::cout << "{" << uv.x << ", " << uv.y << "},\n";
	}
}
