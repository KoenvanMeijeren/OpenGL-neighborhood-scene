#include "object_square.h"

object_square::object_square(const glm::vec3& position, const glm::vec3& light_position, material* material)
: object(position, light_position, material)
{
	init_shape();
	object::init_buffers();
}

object_square::object_square(const glm::vec3& position, const glm::vec3& light_position, material* material,
	const char* texture_path): object(position, light_position, material)
{
	init_shape();
	set_texture(texture_path);
	object::init_buffers();
}

void object_square::init_shape()
{
	vertices_ = {
		{1, 0, 1},
		{-1, 0, -1},
		{-1, 0, 1},
		{1, 0, 1},
		{1, 0, -1},
		{-1, 0, -1},
	};

	normals_ = {
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
	};

	uvs_ = {
		{0, 1},
		{1, 0},
		{1, 1},
		{0, 1},
		{0, 0},
		{1, 0},
	};
}

void object_square::rotate_90_degrees()
{
	for (auto& vertex : vertices_) {
		const float x = vertex.x;
		const float y = vertex.y;
		const float z = vertex.z;

	    vertex.x = 0 * x + 0 * y + 1 * z;
	    vertex.y = 0 * x + 1 * y + 0 * z;
	    vertex.z = -1 * x + 0 * y + 0 * z;
	}

	for (auto& vertex : normals_) {
		const float x = vertex.x;
		const float y = vertex.y;
		const float z = vertex.z;

	    vertex.x = 0 * x + 0 * y + 1 * z;
	    vertex.y = 0 * x + 1 * y + 0 * z;
	    vertex.z = -1 * x + 0 * y + 0 * z;
	}

	object::init_buffers();
}
