#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "configuration.h"
#include "entity.h"
#include "material.h"

class object final: public entity
{
protected:
	glm::mat4 model_view_;

	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> uvs_;

	light_source light_;
	material* material_;
public:
	explicit object(const glm::vec3& position, const glm::vec3& light_position, material* material);
	explicit object(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* object_path);
	explicit object(const glm::vec3& position, const glm::vec3& light_position, material* material, const char* object_path, const char* texture_image_path);
	~object() override;

	// Constructor methods.
	void set_object(const char* object_path);
	void set_texture(const char* texture_image_path) const;

	// Required methods for rendering
	void init_buffers() override;
	void render() override;
};
