#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"

using namespace std;


//--------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------

constexpr int width = 800, height = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint position_id;
GLuint vbo_vertices;

constexpr GLfloat vertices[]
{
    0.5, -0.5, 0.0, 1.0,
    -0.5, -0.5, 0.0, 1.0,
    0.0, 0.5, 0.0, 1.0,
};

constexpr GLfloat colors[]
{
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
};


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27) {
        glutExit();
    }
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    const glm::vec4 blue = glm::vec4(0.0f, 0.0f, 0.4f, 1.0f);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blue));

    // Attach to program_id
    glUseProgram(program_id);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

    // Define an array of generic vertex attribute data
    // ID, size, type, normalized, stride, offset first component
    glVertexAttribPointer(position_id, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Enable array
    glEnableVertexAttribArray(position_id);

    // Stop binding
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Disable array
    glDisableVertexAttribArray(position_id);

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);

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


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    // Generate buffer object (in this case 1)
    glGenBuffers(1, &vbo_vertices);

    // Bind named buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

    // Create and initialize buffer object’s data store
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Location of attribute variable
    position_id = glGetAttribLocation(program_id, "position");

    return 0;
}
