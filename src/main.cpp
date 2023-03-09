// clang-format off
// This is necessary because glew has to be included before freeglut
#include <GL/glew.h>
#include <GL/freeglut.h>
// clang-format on

#include <iostream>
#include "glsl.hpp"

GLuint program_id;

using namespace std;

//--------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------

constexpr int width = 800, height = 600;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboard_handler(const unsigned char key, int a, int b) {
  if (key == 27) {
    glutExit();
  }
}

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void render() {
  // Define background
  static constexpr GLfloat blue[] = {0.0, 0.0, 0.4, 1.0};
  glClearBufferfv(GL_COLOR, 0, blue);

  glBegin(GL_TRIANGLES);

  // lower left corner
  glColor3f(1.0, 1.0, 0.0);
  glVertex2f(-0.5, -0.5);

  // lower right corner
  glColor3f(1.0, 0.0, 0.0);
  glVertex2f(0.5, -0.5);

  // upper right corner
  glColor3f(0.0, 0.0, 1.0);
  glVertex2f(0.5, 0.5);
  glEnd();

  glBegin(GL_TRIANGLES);

  // upper left corner
  glColor3f(0.0, 1.0, 0.0);
  glVertex2f(-0.5, 0.5);

  // lower left corner
  glColor3f(1.0, 1.0, 0.0);
  glVertex2f(-0.5, -0.5);

  // upper right corner
  glColor3f(0.0, 0.0, 1.0);
  glVertex2f(0.5, 0.5);
  glEnd();

  glUseProgram(program_id);

  glColor3f(1.0, 1.0, 0.0);
  glPointSize(40.0f);
  glDrawArrays(GL_POINTS, 0, 1);

  // Swap buffers
  glutSwapBuffers();
}

//------------------------------------------------------------
// void init_glut_glew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void init_glut_glew(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("OpenGL");
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboard_handler);

  glewInit();
}

int main(int argc, char** argv) {
  init_glut_glew(argc, argv);

  // Hide console window
  const HWND h_wnd = GetConsoleWindow();
  ShowWindow(h_wnd, SW_HIDE);

  // Main loop
  glutMainLoop();

  const char* vertex_shader = glsl::readFile("vertexshader.vert");
  const GLuint vsh_id = glsl::makeVertexShader(vertex_shader);

  const char* frag_shader = glsl::readFile("fragmentshader.frag");
  const GLuint fsh_id = glsl::makeFragmentShader(frag_shader);

  program_id = glsl::makeShaderProgram(vsh_id, fsh_id);

  return 0;
}