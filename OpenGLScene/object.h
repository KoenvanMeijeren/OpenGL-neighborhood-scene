#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "animation.h"
#include "camera.h"
#include "configuration.h"
#include "material.h"

__declspec(align(16)) class object
{
private:
	camera *camera_;

	std::vector<animation *> animations_ = {};

	glm::mat4 model_;
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
	~object();

	void* operator new(const size_t size)
	{
	    return _mm_malloc(size, 16);
	}

	void operator delete(void* pointer)
	{
	    _mm_free(pointer);
	}

	// Getters
	glm::mat4 model();

	// Constructor methods.
	void set_object(const char* object_path);
	void set_texture(const char* texture_image_path) const;
	void add_animation(animation* animation);

	// Transformation methods
	void scale(const float x, const float y, const float z);
	void scale(const float scale);
	void rotate(const float rotate_speed, const float x, const float y, const float z);
	void rotate(const float rotate_speed, const float rotate_value);
	void translate(const float x, const float y, const float z);
	void translate(const float translate_value);

	// Required methods for rendering
	void init_buffers() const;
	void render();
};
