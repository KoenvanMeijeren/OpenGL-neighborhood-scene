#include "object_cube.h"

object_cube::object_cube(const glm::vec3& position, const glm::vec3& light_position, material* material)
	:object(position, light_position, material)
{
	init_cube();
	object::init_buffers();
}

object_cube::object_cube(const glm::vec3& position, const glm::vec3& light_position, material* material,
	const char* texture_path): object(position, light_position, material)
{
	init_cube();
	set_texture(texture_path);
	object::init_buffers();
}

void object_cube::init_cube()
{
	vertices_ = {
		// front
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		// bottom
		{0, 0, 0},
		{1, 0, 1},
		{0, 0, 1},
		{0, 0, 0},
		{1, 0, 0},
		{1, 0, 1},
		// left
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 1},
		{0, 0, 0},
		{0, 1, 0},
		{0, 1, 1},
		// right
		{1, 0, 0},
		{1, 0, 1},
		{1, 1, 1},
		{1, 0, 0},
		{1, 1, 0},
		{1, 1, 1},
		// back
		{0, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 0, 1},
		{0, 1, 1},
		{1, 1, 1},
		// top
		{0, 1, 0},
		{1, 1, 0},
		{1, 1, 1},
		{0, 1, 0},
		{0, 1, 1},
		{1, 1, 1},
	};

	uvs_ = {
		// front
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 0},
		{0, 1},
		{1, 1},
		// bottom
		{0, 0},
		{1, 1},
		{0, 1},
		{0, 0},
		{1, 0},
		{1, 1},
		// left
		{1, 0},
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1},
		{0, 1},
		// right
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 0},
		{0, 1},
		{1, 1},
		// back
		{1, 0},
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1},
		{0, 1},
		// top
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 0},
		{0, 1},
		{1, 1},
	};

	normals_ = {
		// front
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		{0, 0, -1},
		// bottom
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		{0, -1, 0},
		// normals for left
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},
		{-1, 0, 0},
		// normals for right
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		{1, 0, 0},
		// normals for back
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		// normals for top
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0},
	};
}
