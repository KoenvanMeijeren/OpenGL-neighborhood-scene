#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "configuration.h"
#include "shader.h"

class material
{
protected:
	material_struct data_;
	shader* shader_;
public:
	explicit material(shader* shader);
	virtual ~material();

	void set_texture_id(const GLuint texture_id) const;
	void init_buffers(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) const;

	// Renders the material by writing the updated values back to the shader (abstract method)
	void render(const glm::mat4& model_view, const glm::mat4& projection, const light_source& light, const std::vector<glm::vec3>& vertices) const;
};
