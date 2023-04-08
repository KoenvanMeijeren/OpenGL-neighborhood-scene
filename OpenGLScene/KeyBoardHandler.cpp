#include "KeyBoardHandler.h"
#include <GL/freeglut.h>

void keyboard_handler(const unsigned char key, int a, int b)
{
    if (key == 27)
    {
        glutExit();
    }
}