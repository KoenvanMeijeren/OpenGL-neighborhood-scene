#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>

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
	const auto teapot_position = glm::vec3(6, 2, 0);
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

    const auto teapot1_position = glm::vec3(0, 5, 0);
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

    const auto torus_position = glm::vec3(3.0, 2.5, 0);
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

 //    const auto car_object_position = glm::vec3(7, 2, 0);
	// const auto car_object_light_position = glm::vec3(1, 1, 1);
 //    material* car_object_material_metal = new material_default(
 //        glm::vec3(0, 0.5, 0), 
 //        glm::vec3(0, 0.5, 0), 
 //        glm::vec3(0.1f),
 //        2024
 //    );
 //    auto *car_object = new object(
 //        car_object_position, 
 //        car_object_light_position,
 //        car_object_material_metal, 
 //        "Objects/car-taxi3.obj"
 //    );

    objects_.push_back(teapot);
    objects_.push_back(teapot1);
    objects_.push_back(torus);
    // Enable this when the scene is completed
    // objects_.push_back(car_object);

    const auto cube_position = glm::vec3(5, 0, 5);
	const auto cube_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* cube_material_metal = new material_lambert(
        glm::vec3(3, 0.3, 0.0), 
        glm::vec3(3, 0, 0), 
        glm::vec3(1),
        1024
    );
    auto *cube = new object_cube(
        cube_position,
        cube_light_position, 
        cube_material_metal
    );
    objects_.push_back(cube);

    object_builders_.push_back(new floor_builder());
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
