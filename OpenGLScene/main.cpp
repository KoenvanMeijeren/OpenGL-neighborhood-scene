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

const char* frag_shader_name = "fragmentshader.frag";
const char* vertex_shader_name = "vertexshader.vert";


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint position_id;
GLuint color_id;
GLuint vbo_vertices;
GLuint vbo_colors;
GLuint vao;


//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------
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

void keyboard_handler(const unsigned char key, int a, int b)
{
    if (key == 27) {
        glutExit();
    }
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void render()
{
    // Define background
    const auto blue = glm::vec4(0.0f, 0.0f, 0.4f, 1.0f);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blue));

    // Attach to program_id
    glUseProgram(program_id);

    // --------------------------------------------------------------------------------
    // VAO generation, replaces the manual VBO creation
    // --------------------------------------------------------------------------------
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // --------------------------------------------------------------------------------
    // VBO for vertices
    // --------------------------------------------------------------------------------
    // // Bind buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    //
    // // Define an array of generic vertex attribute data
    // // ID, size, type, normalized, stride, offset first component
    // glVertexAttribPointer(position_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //
    // // Enable array
    // glEnableVertexAttribArray(position_id);
    //
    // // Stop binding
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // --------------------------------------------------------------------------------
    // VBO for colors
    // --------------------------------------------------------------------------------
    // // Bind buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    //
    // // Define an array of generic vertex attribute data
    // // ID, size, type, normalized, stride, offset first component
    // glVertexAttribPointer(color_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //
    // // Enable array
    // glEnableVertexAttribArray(color_id);
    //
    // // Stop binding
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // --------------------------------------------------------------------------------
    // Draw
    // --------------------------------------------------------------------------------
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void init_glut_glew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard_handler);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void init_shaders()
{
	const char* vertex_shader = glsl::readFile(vertex_shader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertex_shader);

	const char* frag_shader = glsl::readFile(frag_shader_name);
	const GLuint fsh_id = glsl::makeFragmentShader(frag_shader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}

void init_buffers()
{
    // --------------------------------------------------------------------------------
    // Generate buffers for vertices
    // --------------------------------------------------------------------------------
    // Generate buffer object (in this case 1)
    glGenBuffers(1, &vbo_vertices);

    // Bind named buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);

    // Create and initialize buffer object's data store
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Unbind and free buffer for others
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // --------------------------------------------------------------------------------
    // Generate buffers for colors
    // --------------------------------------------------------------------------------
    // Generate buffer object (in this case 1)
    glGenBuffers(1, &vbo_colors);

    // Bind named buffer object
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);

    // Create and initialize buffer object's data store
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Unbind and free buffer for others
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // --------------------------------------------------------------------------------
    // Location of attribute values
    // --------------------------------------------------------------------------------
    position_id = glGetAttribLocation(program_id, "position");
    color_id = glGetAttribLocation(program_id, "color");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glVertexAttribPointer(color_id, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

int main(const int argc, char** argv)
{
    init_glut_glew(argc, argv);
    init_shaders();
    init_buffers();

    // Hide console window
    const HWND h_wnd = GetConsoleWindow();
    ShowWindow(h_wnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
