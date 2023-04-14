#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader_primitive.h"

class object_primitive
{
private:
	camera *camera_;
	shader_primitive *shader_;

	glm::mat4 model_;
	glm::mat4 mvp_;

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
	explicit object_primitive();

	// Required methods for rendering
	void init_buffers() const;
	void render();
};
