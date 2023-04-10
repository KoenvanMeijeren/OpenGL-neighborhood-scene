#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "configuration.h"
#include "glsl.h"
#include "objloader.h"
#include "texture.h"
#include "KeyBoardHandler.h"

//--------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------
struct light_source
{
    glm::vec3 position;
};

struct material
{
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float power{};
};

//--------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------
camera *p_camera = camera::get_instance();

//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------
// Fragment shader names
const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

// ID's
GLuint program_id;
GLuint vao[objects_amount];
GLuint texture_id[objects_amount];

// Uniform ID's
GLint uniform_material_ambient;
GLint uniform_material_diffuse;
GLint uniform_material_specular;
GLint uniform_material_power;
GLint uniform_apply_texture;
GLint uniform_mv;
GLint uniform_projection;
GLint uniform_light_pos;

// Matrices
glm::mat4 model[objects_amount];
glm::mat4 mv[objects_amount];

// Light & materials
light_source light;
material materials[objects_amount];
bool apply_texture[objects_amount];

//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------
vector<glm::vec3> vertices[objects_amount];
vector<glm::vec3> normals[objects_amount];
vector<glm::vec2> uvs[objects_amount];

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------
void render()
{
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if (p_camera->is_drone_mode_enabled)
    {
	    glClearColor(0.0, 0.0, 1.0, 1.0);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Attach to program_id
    glUseProgram(program_id);

    // Before doing anything, update the camera.
    p_camera->update();
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(p_camera->get_projection()));

    // Do transformation (x, y, z)
    model[0] = glm::rotate(model[0], 0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
    model[1] = glm::rotate(model[1], 0.05f, glm::vec3(1.0f, 0.5f, 0.5f));
    model[2] = glm::rotate(model[2], 0.05f, glm::vec3(1.0f, 0.3f, 0.1f));

    for (int index = 0; index < objects_amount; index++)
    {
        // Do transformation
        mv[index] = p_camera->get_view() * model[index];

        // Send mvp
        glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[index]));

        // Bind textures
        glUniform1i(uniform_apply_texture, 0);
        if (apply_texture[index])
        {
            glUniform1i(uniform_apply_texture, 1);
            glBindTexture(GL_TEXTURE_2D, texture_id[index]);
        }

        // Fill uniform vars
        glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(materials[index].ambient_color));
        glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(materials[index].diffuse_color));
        glUniform3fv(uniform_material_specular, 1, glm::value_ptr(materials[index].specular_color));
        glUniform1f(uniform_material_power, materials[index].power);

        // Send vao
        glBindVertexArray(vao[index]);
        glDrawArrays(GL_TRIANGLES, 0, vertices[index].size());
        glBindVertexArray(0);
    }

    // Swap buffers
    glutSwapBuffers();
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
    // glClear(GL_DEPTH_BUFFER_BIT);

    glewInit();
}

//------------------------------------------------------------
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------
void init_shaders()
{
	const char* vertexshader = glsl::readFile(vertexshader_name);
	const GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	const char* fragshader = glsl::readFile(fragshader_name);
	const GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

//------------------------------------------------------------
// Initializes the matrices.
//------------------------------------------------------------
void init_matrices()
{
    model[0] = glm::mat4();
    model[1] = glm::translate(glm::mat4(), glm::vec3(3.0, 2.5, 0.0));
    model[2] = glm::translate(glm::mat4(), glm::vec3(-3.0, -2.5, 0.0));

    for (int index = 0; index < objects_amount; ++index)
    {
        mv[index] = p_camera->get_view() * model[index];
    }
}

//------------------------------------------------------------
// Initializes the objects and textures which will be used.
//------------------------------------------------------------
void init_objects()
{
	// Objects
	loadOBJ("Objects/teapot.obj", vertices[0], uvs[0], normals[0]);
    loadOBJ("Objects/torus.obj", vertices[1], uvs[1], normals[1]);
    loadOBJ("Objects/torus.obj", vertices[2], uvs[2], normals[2]);

    // Textures
    texture_id[0] = loadBMP("Textures/Yellobrk.bmp");
    texture_id[1] = loadBMP("Textures/uvtemplate.bmp");
}

//------------------------------------------------------------
// Initializes the materials and lightning.
//------------------------------------------------------------
void init_materials_light()
{
    light.position = glm::vec3(4.0, 4.0, 4.0);

    // Teapot
    materials[0].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    materials[0].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    materials[0].specular_color = glm::vec3(1);
    materials[0].power = 1024;
    apply_texture[0] = true;

    // Torus, A.K.A. donut
    materials[1].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    materials[1].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    materials[1].specular_color = glm::vec3(1);
    materials[1].power = 4;
    apply_texture[1] = true;

    // Box
    materials[2].ambient_color = glm::vec3(0.3, 0.3, 0.0);
    materials[2].diffuse_color = glm::vec3(0.5, 0.0, 0.5);
    materials[2].specular_color = glm::vec3(1);
    materials[2].power = 1024;
    apply_texture[2] = false;
}

//------------------------------------------------------------
// Allocates and fills buffers.
//------------------------------------------------------------
void init_buffers()
{
	GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_uvs;

    // Get vertex attributes
	const GLuint position_id = glGetAttribLocation(program_id, "position");
	const GLuint normal_id = glGetAttribLocation(program_id, "normal");
	const GLuint uv_id = glGetAttribLocation(program_id, "uv");

    for (int index = 0; index < objects_amount; ++index)
    {
        // vbo for vertices
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices[index].size() * sizeof(glm::vec3), vertices[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for normals
        glGenBuffers(1, &vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, normals[index].size() * sizeof(glm::vec3), normals[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for uv
        glGenBuffers(1, &vbo_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs[index].size() * sizeof(glm::vec2), uvs[index].data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Allocate memory for vao
        glGenVertexArrays(1, &vao[index]);

        // Bind to vao
        glBindVertexArray(vao[index]);

        // Bind vertices to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(position_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind normals to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Bind uv to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
        glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(uv_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Stop bind to vao
        glBindVertexArray(0);
    }

    // Make uniform vars
    uniform_mv = glGetUniformLocation(program_id, "mv");
	uniform_projection = glGetUniformLocation(program_id, "projection");
	uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_material_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_material_power = glGetUniformLocation(program_id, "mat_power");
    uniform_apply_texture = glGetUniformLocation(program_id, "apply_texture");

    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(p_camera->get_projection()));
    glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light.position));
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
    force_mouse_to_enter_screen_in_center();
}

//------------------------------------------------------------
// Initializes the program and runs it.
//------------------------------------------------------------
int main(const int argc, char** argv)
{
    init_glut_glew(argc, argv);
    init_shaders();
    init_matrices();
    init_objects();
    init_materials_light();
    init_buffers();
    hide_console_window();
    glut_pre_main_loop_actions();
    glutMainLoop();

    return EXIT_SUCCESS;
}