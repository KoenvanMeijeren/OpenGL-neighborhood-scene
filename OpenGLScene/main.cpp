#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "configuration.h"
#include "glsl.h"
#include "KeyBoardHandler.h"
#include "scene_manager.h"

//--------------------------------------------------------------------------------
// Prepares the scene and creates the necessary objects.
//--------------------------------------------------------------------------------
camera *p_camera = camera::get_instance();
scene_manager scene_manager(p_camera);

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------
void render()
{
	scene_manager.render();
}

//------------------------------------------------------------
// Render method that is called by the timer function
//------------------------------------------------------------
void render(int n)
{
    glEnable(GL_MULTISAMPLE);
    render();
    glutTimerFunc(delta_time, render, 0);
}

//------------------------------------------------------------
// Initializes Glut and Glew
//------------------------------------------------------------
void init_glut_glew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(render);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(keyboard_handler);
    glutPassiveMotionFunc(mouse_handler);
    glutMouseWheelFunc(mouse_wheel_handler);
    glutTimerFunc(delta_time, render, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glewInit();
}

//------------------------------------------------------------
// General behaviors before running the program.
//------------------------------------------------------------
void hide_console_window()
{
    const HWND h_wnd = GetConsoleWindow();
    ShowWindow(h_wnd, SW_HIDE);
}

// Prevents jumping into the scene.
void force_mouse_to_enter_screen_in_center()
{
    glutWarpPointer(width / 2, height / 2);
}

void glut_pre_main_loop_actions()
{
    hide_console_window();
    force_mouse_to_enter_screen_in_center();
}

//------------------------------------------------------------
// Initializes the program and runs it.
//------------------------------------------------------------
int main(const int argc, char** argv)
{
    init_glut_glew(argc, argv);
    scene_manager.init();
    glut_pre_main_loop_actions();
    glutMainLoop();

    return EXIT_SUCCESS;
}