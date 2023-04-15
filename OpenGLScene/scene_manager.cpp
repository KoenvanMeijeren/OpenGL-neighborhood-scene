#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>

#include "animation_move_in_range.h"
#include "animation_rotate.h"
#include "objloader.h"
#include "camera.h"
#include "floor-builder.h"
#include "glsl.h"
#include "material_default.h"
#include "material_lambert.h"
#include "line_shape_cube.h"
#include "material_metal.h"
#include "object_cube.h"
#include "object_square.h"
#include "primitive-person-builder.h"
#include "road-builder.h"

scene_manager::scene_manager()
{
	camera_ = camera::get_instance();
}

scene_manager::~scene_manager()
{
    delete camera_;
    for (const auto object : objects_)
    {
	    delete object;
    }

    for (const auto object_builder : object_builders_)
    {
	    delete object_builder;
    }
}

void scene_manager::init()
{
	const auto teapot_position = glm::vec3(6, 2, -3);
	const auto teapot_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* teapot_material = new material_default(
        glm::vec3(1.0, 0.0, 0.0), 
        glm::vec3(1.0, 1.0, 0.0), 
        glm::vec3(1), 
        1024
    );
    auto *teapot = new object(
        teapot_position, 
        teapot_light_position, 
        teapot_material,
        "Objects/teapot.obj"
    );
    teapot->add_animation(new animation_rotate(0.01f, 0.5f, 1.0f, 0.2f));

    const auto teapot1_position = glm::vec3(3, 0, -5);
	const auto teapot1_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* teapot1_material = new material_default(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        1
    );
    auto *teapot1 = new object(
        teapot1_position, 
        teapot1_light_position, 
        teapot1_material,
        "Objects/teapot.obj",
        "Textures/Yellobrk.bmp"
    );

    const auto torus_position = glm::vec3(-5, 2.5, -5);
	const auto torus_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* torus_material = new material_default(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        4
    );
    auto *torus = new object(
        torus_position, 
        torus_light_position, 
        torus_material,
        "Objects/torus.obj",
        "Textures/uvtemplate.bmp"
    );
    torus->add_animation(new animation_rotate(0.05f, 1.0f, 0.5f, 0.5f));

    const auto car_object_position = glm::vec3(1, 0, 0.2f);
	const auto car_object_light_position = glm::vec3(1, 1, 1);
    material* car_object_material_metal = new material_default(
        glm::vec3(0.1, 0.5, 0.3), 
        glm::vec3(0.1, 0.7, 0.4), 
        glm::vec3(0.1f),
        2024
    );
    auto *car_object = new object(
        car_object_position, 
        car_object_light_position,
        car_object_material_metal, 
        "Objects/car-taxi3.obj"
    );
    car_object->rotate(1.55f, 0, 0.2f, 0);
    car_object->scale(0.6f);
    car_object->translate(0, 0, -35);
    car_object->add_animation(new animation_move_in_range(0.2f, 0, 0, 65));
 
    const auto car_reversed_object_position = glm::vec3(20, 0, 2.4);
	const auto car_reversed_object_light_position = glm::vec3(1, 1, 1);
    material* car_reversed_object_material_metal = new material_default(
        glm::vec3(1, 0, 0), 
        glm::vec3(1, 1.0, 0), 
        glm::vec3(0.1f),
        2024
    );
    auto *car_reversed_object = new object(
        car_reversed_object_position, 
        car_reversed_object_light_position,
        car_reversed_object_material_metal, 
        "Objects/car-taxi3.obj"
    );
    car_reversed_object->rotate(4.7f, 0, 0.5f, 0);
    car_reversed_object->scale(0.6f);
    car_reversed_object->add_animation(new animation_move_in_range(0.2f, 0, 0, 65));

    objects_.push_back(teapot);
    objects_.push_back(teapot1);
    objects_.push_back(torus);
    objects_.push_back(car_object);
    objects_.push_back(car_reversed_object);

    object_builders_.push_back(new floor_builder());
    object_builders_.push_back(new road_builder());
    object_builders_.push_back(new primitive_person_builder());
	for (const auto object_builder : object_builders_)
	{
		object_builder->init();
	}
}

void scene_manager::render() const
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if (camera_->is_drone_mode_enabled)
    {
	    glClearColor(0.0, 0.0, 1.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto object : objects_)
    {
	    object->render();
    }

    for (const auto object_builder : object_builders_)
	{
		object_builder->render();
	}

    glutSwapBuffers();
}
