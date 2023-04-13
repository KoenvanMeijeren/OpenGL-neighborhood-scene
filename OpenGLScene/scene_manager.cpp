#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>

#include "objloader.h"
#include "camera.h"
#include "glsl.h"

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
    auto *teapot = new object(0, 0, 0);
    teapot->set_object("Objects/teapot.obj");
    teapot->set_texture("Textures/Yellobrk.bmp");
    teapot->set_light(glm::vec3(4.0, 4.0, 4.0));
    teapot->set_material(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        1024
    );
    teapot->init_buffers();

    auto *torus = new object(3.0, 2.5, 0.0);
    torus->set_object("Objects/torus.obj");
    torus->set_texture("Textures/uvtemplate.bmp");
    torus->set_light(glm::vec3(4.0, 4.0, 4.0));
    torus->set_material(
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3(1), 
        4
    );
    torus->init_buffers();

    auto *box = new object(-3.0, -2.5, 0.0);
    box->set_object("Objects/box.obj");
    box->set_light(glm::vec3(4.0, 4.0, 4.0));
    box->set_material(
        glm::vec3(0.3, 0.3, 0.0), 
        glm::vec3(0.5, 0.0, 0.5), 
        glm::vec3(1), 
        1024
    );
    box->init_buffers();

    objects_.push_back(teapot);
    objects_.push_back(torus);
    objects_.push_back(box);
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
