#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "animation.h"
#include "camera.h"
#include "shader_primitive.h"

__declspec(align(16)) class object_primitive final
{
private:
	camera *camera_;
	shader_primitive *shader_;
	std::vector<animation *> animations_ = {};

	glm::mat4 model_;
	glm::mat4 model_view_projection_;

	//------------------------------------------------------------
	//
	//           7----------6
	//          /|         /|
	//         / |        / |
	//        /  4-------/--5               y
	//       /  /       /  /                |
	//      3----------2  /                 ----x
	//      | /        | /                 /
	//      |/         |/                  z
	//      0----------1
	//------------------------------------------------------------

	// Vertices
	GLfloat vertices_[72] = {
	    // front
	    -1.0, -1.0, 1.0,
	    1.0, -1.0, 1.0,
	    1.0, 1.0, 1.0,
	    -1.0, 1.0, 1.0,
	    // back
	    -1.0, -1.0, -1.0,
	    1.0, -1.0, -1.0,
	    1.0, 1.0, -1.0,
	    -1.0, 1.0, -1.0,
	};

	// Colors
	GLfloat colors_[72] = {
	    // front colors
	    1.0, 1.0, 0.0,
	    0.0, 1.0, 0.0,
	    0.0, 0.0, 1.0,
	    1.0, 1.0, 1.0,
	    // back colors
	    0.0, 1.0, 1.0,
	    1.0, 0.0, 1.0,
	    1.0, 0.0, 0.0,
	    1.0, 1.0, 0.0,
	};

	// Elements
	GLushort cube_elements_[72] = {
	    0,1,1,2,2,3,3,0,  // front
	    0,4,1,5,3,7,2,6,  // front to back
	    4,5,5,6,6,7,7,4   // back
	};
public:
	explicit object_primitive(const float x, const float y, const float z);
	~object_primitive();

	void* operator new(const size_t size)
	{
	    return _mm_malloc(size, 16);
	}

	void operator delete(void* pointer)
	{
	    _mm_free(pointer);
	}

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
	void init_buffers() const;
	void render();
};
