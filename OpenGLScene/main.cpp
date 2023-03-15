#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glsl.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint vao;

// Uniform
GLuint uniform_mvp; // n

//------------------------------------------------------------
// Variables for object
//
//           7----------6
//          /|         /|
//         / |        / |
//        /  4-------/--5               y
//       /  /       /  /                |
//      3----------2  /                 ----x
//      | /        | /                 /
//      |/         |/                  z
//      0----------1
//------------------------------------------------------------

// GLfloat vertices[] = {
//     // front
//     -1.0, -1.0, 1.0,
//     1.0, -1.0, 1.0,
//     1.0, 1.0, 1.0,
//     -1.0, 1.0, 1.0,
//     // back
//     -1.0, -1.0, -1.0,
//     1.0, -1.0, -1.0,
//     1.0, 1.0, -1.0,
//     -1.0, 1.0, -1.0
// };
//
// GLfloat colors[] = {
//     // front colors
//     1.0, 1.0, 0.0,
//     0.0, 1.0, 0.0,
//     0.0, 0.0, 1.0,
//     1.0, 1.0, 1.0,
//     // back colors
//     0.0, 1.0, 1.0,
//     1.0, 0.0, 1.0,
//     1.0, 0.0, 0.0,
//     1.0, 1.0, 0.0
// };
//
// GLushort cube_elements[] = {
//     0,1,1,2,2,3,3,0,  // front
//     0,4,1,5,3,7,2,6,  // front to back
//     4,5,5,6,6,7,7,4   //back
// };

// Vertices
const GLfloat vertices[]
{
    0.5, -0.5, 0.0,
    -0.5, -0.5, 0.0,
    0.0, 0.5, 0.0
};

// Colors
const GLfloat colors[]
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

//--------------------------------------------------------------------------------
// Matrix transformations
//--------------------------------------------------------------------------------
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Animation preparations
    model = glm::rotate(model, 0.01f, glm::vec3(0.0, 1.0, 0.0));
    mvp = projection * view * model;

    // Attach to program_id
    glUseProgram(program_id);

    // Set this command in the render method when creating an animation.
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    // Send vao
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

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
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
    GLuint position_id;
    GLuint color_id;
    GLuint vbo_vertices;
    GLuint vbo_colors;

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for colors
    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get vertex attributes
    position_id = glGetAttribLocation(program_id, "position");
    color_id = glGetAttribLocation(program_id, "color");

    // Allocate memory for vao
    glGenVertexArrays(1, &vao);

    // Bind to vao
    glBindVertexArray(vao);

    // Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind colors to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Stop bind to vao
    glBindVertexArray(0);

    // Get and fill uniform variables
    uniform_mvp = glGetUniformLocation(program_id, "mvp");
    // Uncomment when not using an animation 
    // glUseProgram(program_id);
    // glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void InitMatrices()
{
    // Identity matrix
    model = glm::mat4();

    view = glm::lookAt(
        glm::vec3(0.0, 0.0, 2.0),
        // glm::vec3(2.0, 0.5, 2.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0)
    );

    projection = glm::perspective(
		glm::radians(45.0f),
        1.0f * 800.0f / 600.0f,
        0.1f,
        20.0f
    );

    mvp = projection * view * model;
}

int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();
    InitBuffers();
    InitMatrices();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
