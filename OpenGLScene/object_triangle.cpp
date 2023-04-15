#include "object_triangle.h"

object_triangle::object_triangle(const glm::vec3& position, const glm::vec3& light_position, material* material)
: object(position, light_position, material)
{
	init_shape();
	object::init_buffers();
}

object_triangle::object_triangle(const glm::vec3& position, const glm::vec3& light_position, material* material,
	const char* texture_path): object(position, light_position, material)
{
	init_shape();
	set_texture(texture_path);
	object::init_buffers();
}

void object_triangle::init_shape()
{
	vertices_ = {
		{1, 0, 1},
		{-1, 0, -1},
		{-1, 0, 1},
	};

	uvs_ = {
		{1, -0},
		{0, -1},
		{0, -0},
	};

	normals_ = {
		{-0, 1, -0},
		{-0, 1, -0},
		{-0, 1, -0},
	};
}
