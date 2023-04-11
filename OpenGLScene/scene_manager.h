#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_vec.hpp>

#include "Camera.h"
#include "configuration.h"

class scene_manager
{
private:
	camera* camera_;

	std::vector<glm::vec3> vertices[objects_amount];
	std::vector<glm::vec3> normals[objects_amount];
	std::vector<glm::vec2> uvs[objects_amount];

	// ID's
	GLuint program_id{};
	GLuint vao[objects_amount]{};
	GLuint texture_id[objects_amount]{};

	// Uniform ID's
	GLint uniform_material_ambient{};
	GLint uniform_material_diffuse{};
	GLint uniform_material_specular{};
	GLint uniform_material_power{};
	GLint uniform_apply_texture{};
	GLint uniform_mv{};
	GLint uniform_projection{};
	GLint uniform_light_pos{};

	// Matrices
	glm::mat4 models[objects_amount];
	glm::mat4 mvs[objects_amount];

	// Light & materials
	light_source light;
	material materials[objects_amount];
	bool apply_texture[objects_amount]{};

	// Initializes the objects and textures which will be used.
	void create_mesh_objects();
public:
	// Creates the objects and fills the needed variables.
	explicit scene_manager(camera *camera);

	// Initializes the fragmentshader and vertexshader
	void init_shaders();

	// Initializes the matrices.
	void init_matrices();

	// Initializes the objects and textures which will be used.
	void init_objects();

	// Initializes the materials and lightning.
	void init_materials_light();

	// Allocates and fills buffers.
	void init_buffers();

	// Initializes the object manager and prepares it for rendering.
	void init();

	// Renders the world.
	void render();
};
