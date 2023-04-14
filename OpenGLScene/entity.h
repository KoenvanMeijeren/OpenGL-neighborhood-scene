#pragma once
#include <malloc.h>
#include <vector>

#include "animation.h"
#include "camera.h"

__declspec(align(16)) class entity
{
protected:
	camera *camera_;

	std::vector<animation *> animations_ = {};

	glm::mat4 model_;
public:
	entity();
	virtual ~entity();

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

	// Constructor methods
	void add_animation(animation* animation);

	// Transformation methods
	void scale(const float x, const float y, const float z);
	void scale(const float scale);
	void rotate(const float rotate_speed, const float x, const float y, const float z);
	void rotate(const float rotate_speed, const float rotate_value);
	void translate(const float x, const float y, const float z);
	void translate(const float translate_value);

	// Required methods for rendering
	virtual void init_buffers() = 0;
	virtual void render() = 0;
};
