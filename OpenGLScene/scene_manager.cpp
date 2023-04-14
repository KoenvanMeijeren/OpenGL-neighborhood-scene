#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>

#include "animation_rotate.h"
#include "objloader.h"
#include "camera.h"
#include "glsl.h"
#include "material_default.h"
#include "material_lambert.h"
#include "material_metal.h"
#include "object_cube.h"
#include "line_shape.h"

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

    for (const auto line_shape_object : line_shape_objects_)
    {
	    delete line_shape_object;
    }
}

void scene_manager::init()
{
	const auto teapot_position = glm::vec3(0, 0, 0);
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

    const auto box_position = glm::vec3(-3.0, -2.5, 0);
	const auto box_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* box_material = new material_default(
        glm::vec3(0.3, 0.3, 0.0), 
        glm::vec3(0.5, 0, 0.5), 
        glm::vec3(1), 
        2
    );
    auto *box = new object(
        box_position, 
        box_light_position, 
        box_material, 
        "Objects/box.obj"
    );
    box->add_animation(new animation_rotate(0.05f, 1.0f, 0, 0));

    const auto box2_position = glm::vec3(-3.0, 2.5, 0);
	const auto box2_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* box_material_lambert = new material_lambert(
        glm::vec3(0.3, 0.3, 0.0), 
        glm::vec3(0.5, 0, 0.5), 
        glm::vec3(1),
        1
    );
    auto *box2 = new object(
        box2_position, 
        box2_light_position,
        box_material_lambert, 
        "Objects/box.obj"
    );

    const auto box3_position = glm::vec3(-3.0, -7.5, 0);
	const auto box3_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* box_material_metal = new material_metal(
        glm::vec3(1, 0, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(1),
        1,
        1,
        0.5
    );
    auto *box3 = new object(
        box3_position, 
        box3_light_position,
        box_material_metal, 
        "Objects/box.obj"
    );

    objects_.push_back(teapot);
    objects_.push_back(teapot1);
    objects_.push_back(torus);
    objects_.push_back(box);
    objects_.push_back(box2);
    objects_.push_back(box3);

    auto *line_shape_object = new line_shape(3, -3, 0);
    line_shape_object->add_animation(new animation_rotate(0.01f, 0.0f, 1.0f, 0.0f));
    line_shape_objects_.push_back(line_shape_object);

    const auto cube_position = glm::vec3(5, -5, 0);
	const auto cube_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* cube_material_metal = new material_metal(
        glm::vec3(3, 0.3, 0.0), 
        glm::vec3(3, 0, 0), 
        glm::vec3(1),
        1,
        1,
        0.5
    );
    auto *cube = new object_cube(
        cube_position,
        cube_light_position, 
        cube_material_metal,
        "Textures/uvtemplate.bmp"
    );
    objects_.push_back(cube);

    const auto cube2_position = glm::vec3(10, -5, 0);
	const auto cube2_light_position = glm::vec3(1.0, 1.0, 1.0);
    material* cube2_material_metal = new material_default(
        glm::vec3(5, 5, 0), 
        glm::vec3(5, 5, 0), 
        glm::vec3(1),
        1
    );
    auto *cube2 = new object_cube(
        cube2_position,
        cube2_light_position, 
        cube2_material_metal
    );
    objects_.push_back(cube2);
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

    for (const auto line_shape_object : line_shape_objects_)
    {
	    line_shape_object->render();
    }

    glutSwapBuffers();
}
