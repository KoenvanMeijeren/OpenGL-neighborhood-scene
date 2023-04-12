#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "configuration.h"
#include "shader_manager.h"

class object
{
private:
	camera *camera_;
	shader_manager shader_;

	glm::mat4 model_, original_model_;
	glm::mat4 mv_;

	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> uvs_;

	light_source light_;
	material material_;
	bool apply_texture_ = false;

	GLuint vao_{};
	GLuint texture_id_{};

	// Uniform ID's
	GLint uniform_material_ambient_{};
	GLint uniform_material_diffuse_{};
	GLint uniform_material_specular_{};
	GLint uniform_material_power_{};
	GLint uniform_apply_texture_{};
	GLint uniform_mv_{};
	GLint uniform_projection_{};
	GLint uniform_light_pos_{};
public:
	explicit object(const float x, const float y, const float z);

	void set_object(const char* object_path);
	void set_texture(const char* texture_image_path);
	void set_light(const glm::vec3& light_position);
	void set_material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, float power);
	void init_buffers();
	void render();
};
