#include "floor-builder.h"

#include "material_default.h"
#include "object_square.h"

void floor_builder::init()
{
	const auto square_position = glm::vec3(0, 0, 0);
	const auto square_light_position = glm::vec3(1.0, 1.0, 1.0);
    material* square_material_metal = new material_default(
        glm::vec3(0, 0.5, 0), 
        glm::vec3(0, 0.5, 0), 
        glm::vec3(0),
        1024
    );

	for (int position_x = -10; position_x <= 10; ++position_x)
	{
		for (int position_z = -10; position_z <= 10; ++position_z)
		{
			auto *square = new object_square(
		        square_position,
		        square_light_position, 
		        square_material_metal,
				"Textures/grass.bmp"
		    );
		    square->translate(static_cast<float>(position_x * 2), 0, static_cast<float>(position_z * 2));
		    objects_.push_back(square);
		}
	}
}