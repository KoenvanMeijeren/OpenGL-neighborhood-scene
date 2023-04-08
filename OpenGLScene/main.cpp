#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "glsl.h"
#include "objloader.h"
#include "texture.h"

using namespace std;


//--------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------
struct LightSource
{
    glm::vec3 position;
};

struct Material
{
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float power;
};

//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

constexpr int width = 800, height = 600, objects_amount = 3;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int DELTA_TIME = 10;

//--------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------
camera camera(width, height);

//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint vao[objects_amount];
GLuint texture_id[objects_amount];

GLuint uniform_material_ambient;
GLuint uniform_material_diffuse;
GLuint uniform_material_specular;
GLuint uniform_material_power;
GLuint uniform_apply_texture;

// Uniform ID's
GLuint uniform_mv;

// Matrices
glm::mat4 model[objects_amount];
glm::mat4 mv[objects_amount];

// Light & materials
LightSource light;
Material material[objects_amount];
bool apply_texture[objects_amount];

//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------
vector<glm::vec3> vertices[objects_amount];
vector<glm::vec3> normals[objects_amount];
vector<glm::vec2> uvs[objects_amount];

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
    {
        glutExit();
    }
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do transformation (x, y, z)
    model[0] = glm::rotate(model[0], 0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
    model[1] = glm::rotate(model[1], 0.05f, glm::vec3(1.0f, 0.5f, 0.5f));
    model[2] = glm::rotate(model[2], 0.05f, glm::vec3(1.0f, 0.3f, 0.1f));

    // Attach to program_id
    glUseProgram(program_id);

    for (int index = 0; index < objects_amount; index++)
    {
        // Do transformation
        mv[index] = camera.get_view() * model[index];

        // Send mvp
        glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[index]));

        // Bind textures
        // glBindTexture(GL_TEXTURE_2D, texture_id[index]);
        glUniform1i(uniform_apply_texture, 0);
        if (apply_texture[index])
        {
            glUniform1i(uniform_apply_texture, 1);
            glBindTexture(GL_TEXTURE_2D, texture_id[index]);
        }

        // Fill uniform vars
        glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(material[index].ambient_color));
        glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(material[index].diffuse_color));
        glUniform3fv(uniform_material_specular, 1, glm::value_ptr(material[index].specular_color));
        glUniform1f(uniform_material_power, material[index].power);

        // Send vao
        glBindVertexArray(vao[index]);
        glDrawArrays(GL_TRIANGLES, 0, vertices[index].size());
        glBindVertexArray(0);
    }

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    glEnable(GL_MULTISAMPLE);
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    // glClear(GL_DEPTH_BUFFER_BIT);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
    char* vertexshader = glsl::readFile(vertexshader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertexshader);

    char* fragshader = glsl::readFile(fragshader_name);
    GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

//------------------------------------------------------------
// void InitMatrices()
//------------------------------------------------------------

void InitMatrices()
{
    model[0] = glm::mat4();
    model[1] = glm::translate(glm::mat4(), glm::vec3(3.0, 0.5, 0.0));
    model[2] = glm::translate(glm::mat4(), glm::vec3(-3.0, 0.5, 0.0));

    for (int index = 0; index < objects_amount; ++index)
    {
        mv[index] = camera.get_view() * model[index];
    }
}

//------------------------------------------------------------
// void InitObjects()
//------------------------------------------------------------

void InitObjects()
{
    bool res;

    // Objects
    res = loadOBJ("Objects/teapot.obj", vertices[0], uvs[0], normals[0]);
    res = loadOBJ("Objects/torus.obj", vertices[1], uvs[1], normals[1]);
    res = loadOBJ("Objects/torus.obj", vertices[2], uvs[2], normals[2]);

    // Textures
    texture_id[0] = loadBMP("Textures/Yellobrk.bmp");
    texture_id[1] = loadBMP("Textures/uvtemplate.bmp");
}

//------------------------------------------------------------
// void InitMaterialsLight()
//------------------------------------------------------------

void InitMaterialsLight()
{
    light.position = glm::vec3(4.0, 4.0, 4.0);

    // Teapot
    material[0].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    material[0].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    material[0].specular_color = glm::vec3(1);
    material[0].power = 1024;
    apply_texture[0] = true;

    // Torus, A.K.A. donut
    material[1].ambient_color = glm::vec3(0.0, 0.0, 0.0);
    material[1].diffuse_color = glm::vec3(0.0, 0.0, 0.0);
    material[1].specular_color = glm::vec3(1);
    material[1].power = 4;
    apply_texture[1] = true;

    // Box
    material[2].ambient_color = glm::vec3(0.3, 0.3, 0.0);
    material[2].diffuse_color = glm::vec3(0.5, 0.0, 0.5);
    material[2].specular_color = glm::vec3(1);
    material[2].power = 1024;
    apply_texture[2] = false;
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
    GLuint position_id;
    GLuint normal_id;
    GLuint uv_id;
    GLuint vbo_vertices;
    GLuint vbo_normals;
    GLuint vbo_uvs;

    // Get vertex attributes
    position_id = glGetAttribLocation(program_id, "position");
    normal_id = glGetAttribLocation(program_id, "normal");
    uv_id = glGetAttribLocation(program_id, "uv");

    for (int index = 0; index < objects_amount; ++index)
    {
        // vbo for vertices
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, vertices[index].size() * sizeof(glm::vec3), &vertices[index][0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for normals
        glGenBuffers(1, &vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, normals[index].size() * sizeof(glm::vec3), &normals[index][0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for uv
        glGenBuffers(1, &vbo_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
        glBufferData(GL_ARRAY_BUFFER, uvs[index].size() * sizeof(glm::vec2), &uvs[index][0], GL_STATIC_DRAW);
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
    GLuint uniform_projection = glGetUniformLocation(program_id, "projection");
    GLuint uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_material_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_material_power = glGetUniformLocation(program_id, "mat_power");
    uniform_apply_texture = glGetUniformLocation(program_id, "apply_texture");

    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(camera.get_projection()));
    glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light.position));
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();
    InitMatrices();
    InitObjects();
    InitMaterialsLight();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}