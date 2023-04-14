#pragma once
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "entity.h"
#include "shader_line_shape.h"

class line_shape_rectangle final: public entity
{
private:
	shader_line_shape *shader_;

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
	};

	// Colors
	GLfloat colors_[72] = {
	    // front colors
	    1.0, 1.0, 0.0,
	    1.0, 1.0, 0.0,
	    1.0, 1.0, 0.0,
	    1.0, 1.0, 0.0,
	};

	// Elements
	GLushort cube_elements_[72] = {
	    0,1,1,2,2,3,3,0,  // front
	};
public:
	explicit line_shape_rectangle(const float x, const float y, const float z);
	~line_shape_rectangle() override;

	// Required methods for rendering
	void init_buffers() override;
	void render() override;
};
