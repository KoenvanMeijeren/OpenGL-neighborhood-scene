#include "leaves-builder.h"

#include "material_default.h"
#include "object_square.h"

void leaves_builder::init()
{
	const auto square_position = glm::vec3(0, 0, 0);
	const auto square_light_position = glm::vec3(1.0, 1.0, 1.0);
    material* square_material_metal = new material_default(
        glm::vec3(0.3, 0.3, 0.3), 
        glm::vec3(0.3, 0.3, 0.3), 
        glm::vec3(0),
        1024
    );

	for (int position_x = -10; position_x <= 10; ++position_x)
	{
		auto *square_left_to_road = new object_square(
	        square_position,
	        square_light_position, 
	        square_material_metal,
			"Textures/leaves.bmp"
	    );
	    square_left_to_road->translate(static_cast<float>(position_x * 2), 0.01f, -2);
	    objects_.push_back(square_left_to_road);

		auto *square_right_to_road = new object_square(
	        square_position,
	        square_light_position, 
	        square_material_metal,
			"Textures/leaves.bmp"
	    );
	    square_right_to_road->translate(static_cast<float>(position_x * 2), 0.01f, 4);
	    objects_.push_back(square_right_to_road);
	}
}
