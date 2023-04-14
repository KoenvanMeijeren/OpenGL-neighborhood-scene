#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>

#include "animation_rotate.h"
#include "objloader.h"
#include "camera.h"
#include "glsl.h"
#include "material_default.h"
#include "material_lambert.h"

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
}

void scene_manager::init()
{
	const auto teapot_position = glm::vec3(0, 0, 0);
	const auto teapot_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* teapot_material = new material_default(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        1024
    );
    auto *teapot = new object(teapot_position, teapot_light_position, teapot_material);
    teapot->set_object("Objects/teapot.obj");
    teapot->set_texture("Textures/Yellobrk.bmp");
    teapot->init_buffers();
    teapot->add_animation(new animation_rotate(0.01f, 0.5f, 1.0f, 0.2f));

    const auto teapot1_position = glm::vec3(0, 5, 0);
	const auto teapot1_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* teapot1_material = new material_default(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        1
    );
    auto *teapot1 = new object(teapot1_position, teapot1_light_position, teapot1_material);
    teapot1->set_object("Objects/teapot.obj");
    teapot1->set_texture("Textures/Yellobrk.bmp");
    teapot1->init_buffers();

    const auto torus_position = glm::vec3(3.0, 2.5, 0);
	const auto torus_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* torus_material = new material_default(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        4
    );
    auto *torus = new object(torus_position, torus_light_position, torus_material);
    torus->set_object("Objects/torus.obj");
    torus->set_texture("Textures/uvtemplate.bmp");
    torus->init_buffers();
    torus->add_animation(new animation_rotate(0.05f, 1.0f, 0.5f, 0.5f));

    const auto box_position = glm::vec3(-3.0, -2.5, 0);
	const auto box_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* box_material = new material_default(
        glm::vec3(0.3, 0.3, 0.0), 
        glm::vec3(0.5, 0, 0.5), 
        glm::vec3(1), 
        2
    );
    auto *box = new object(box_position, box_light_position, box_material);
    box->set_object("Objects/box.obj");
    box->init_buffers();
    box->add_animation(new animation_rotate(0.05f, 1.0f, 0, 0));

    const auto box2_position = glm::vec3(-3.0, 2.5, 0);
	const auto box2_light_position = glm::vec3(4.0, 4.0, 4.0);
    material* box_material_lambert = new material_lambert(
        glm::vec3(0.3, 0.3, 0.0), 
        glm::vec3(0.5, 0, 0.5), 
        glm::vec3(1),
        1
    );
    auto *box2 = new object(box2_position, box2_light_position, box_material_lambert);
    box2->set_object("Objects/box.obj");
    box2->init_buffers();

    objects_.push_back(teapot);
    objects_.push_back(teapot1);
    objects_.push_back(torus);
    objects_.push_back(box);
    objects_.push_back(box2);
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

    glutSwapBuffers();
}
