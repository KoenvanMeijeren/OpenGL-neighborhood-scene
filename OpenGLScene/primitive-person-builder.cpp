#include "primitive-person-builder.h"

#include "line_shape_cube.h"
#include "line_shape_rectangle.h"
#include "material_default.h"
#include "material_lambert.h"
#include "object_bar.h"
#include "object_cube.h"

void primitive_person_builder::init()
{
	const auto legs_position = glm::vec3(5, 0, 5);
	const auto legs_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* legs_material_metal = new material_lambert(
        glm::vec3(3, 0.3, 0.0), 
        glm::vec3(3, 0, 0), 
        glm::vec3(1),
        1024
    );
    auto *legs = new object_cube(
        legs_position,
        legs_light_position, 
        legs_material_metal
    );
    objects_.push_back(legs);

    const auto body_position = glm::vec3(5.5, 1.5, 5.5);
	const auto body_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* body_material_metal = new material_default(
        glm::vec3(3, 0.3, 0.0), 
        glm::vec3(3, 0, 0), 
        glm::vec3(1),
        1024
    );
    auto *body = new object_bar(
        body_position,
        body_light_position, 
        body_material_metal
    );
    body->scale(0.5);
    body->rotate(1.55f, 0, 0.5f, 0);
    objects_.push_back(body);

    auto *head = new line_shape_cube(5.5f, 2.5, 5.5f);
    head->scale(0.5);
    objects_.push_back(head);

    auto *body_clothes = new line_shape_rectangle(5.5f, 1.5, 5.52f);
    body_clothes->scale(0.5);
    objects_.push_back(body_clothes);
}
