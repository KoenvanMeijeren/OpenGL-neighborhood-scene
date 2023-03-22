#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objloader.h"
#include <vector>

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
GLuint vbo_normals;

// Uniform ID's
GLuint uniform_mvp;
GLuint uniform_mv;

// Matrices
glm::mat4 model, view, projection;
glm::mat4 mvp;
glm::mat4 mv;


vector<glm::vec3> normals;
vector<glm::vec3> vertices;
vector<glm::vec2> uvs;



//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------

//------------------------------------------------------------
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

// Vertices
//const GLfloat vertices[] = {
//    // front
//    -1.0, -1.0, 1.0,
//    1.0, -1.0, 1.0,
//    1.0, 1.0, 1.0,
//    -1.0, 1.0, 1.0,
//    // back
//    -1.0, -1.0, -1.0,
//    1.0, -1.0, -1.0,
//    1.0, 1.0, -1.0,
//    -1.0, 1.0, -1.0,
//};

//// Colors
//const GLfloat colors[] = {
//    // front colors
//    1.0, 1.0, 0.0,
//    0.0, 1.0, 0.0,
//    0.0, 0.0, 1.0,
//    1.0, 1.0, 1.0,
//    // back colors
//    0.0, 1.0, 1.0,
//    1.0, 0.0, 1.0,
//    1.0, 0.0, 0.0,
//    1.0, 1.0, 0.0,
//};

// Elements
GLushort cube_elements[] = {
    0,1,1,2,2,3,3,0,  // front
    0,4,1,5,3,7,2,6,  // front to back
    4,5,5,6,6,7,7,4   // back
};


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

    // Do transformation
    model = glm::rotate(model, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = projection * view * model;

    // Attach to program_id
    glUseProgram(program_id);

    // Send mvp
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    //// Send vao
    //glBindVertexArray(vao);
    //glDrawElements(GL_LINES, sizeof(cube_elements) / sizeof(GLushort),
    //    GL_UNSIGNED_SHORT, 0);
    //glBindVertexArray(0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

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
    model = glm::mat4();
    view = glm::lookAt(
        glm::vec3(2.0, 2.0, 7.0),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0));
    projection = glm::perspective(
        glm::radians(45.0f),
        1.0f * WIDTH / HEIGHT, 0.1f,
        20.0f);
    mvp = projection * view * model;
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
    GLuint position_id;
    //GLuint color_id;
    GLuint vbo_vertices;
    //GLuint vbo_colors;
    //GLuint ibo_elements;

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for normals
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER,
        normals.size() * sizeof(glm::vec3),
        &normals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for colors
    //glGenBuffers(1, &vbo_colors);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo for elements
    //glGenBuffers(1, &ibo_elements);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements),
    //    cube_elements, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Get vertex attributes
    position_id = glGetAttribLocation(program_id, "position");
    GLuint normal_id = glGetAttribLocation(program_id, "normal");
    //color_id = glGetAttribLocation(program_id, "5color");

    // Allocate memory for vao
    glGenVertexArrays(1, &vao);

    // Bind to vao
    glBindVertexArray(vao);

    //Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind normals to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::vec3 light_position, ambient_color, diffuse_color;

    // Make uniform vars
    uniform_mv = glGetUniformLocation(program_id, "mv");
    GLuint uniform_proj = glGetUniformLocation(program_id, "projection");
    GLuint uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    GLuint uniform_material_ambient = glGetUniformLocation(program_id,
        "mat_ambient");
    GLuint uniform_material_diffuse = glGetUniformLocation(program_id,
        "mat_diffuse");

    // Fill uniform vars
    glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
    glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(ambient_color));
    glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(diffuse_color));

    // Bind colors to vao
    //glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    //glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(color_id);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind elements to vao
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);

    // Stop bind to vao
    glBindVertexArray(0);

    // Make uniform vars
    uniform_mvp = glGetUniformLocation(program_id, "mvp");

    // Define model
    mvp = projection * view * model;

    // Send mvp
    glUseProgram(program_id);
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void InitObjects() {

    bool res;
    res = loadOBJ("teapot.obj", vertices, uvs, normals);
    std::cout << res << endl;
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();
    InitMatrices();
    InitObjects();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}