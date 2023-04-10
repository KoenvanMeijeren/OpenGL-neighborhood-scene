#include "KeyBoardHandler.h"
#include <GL/freeglut.h>

#include "Camera.h"
#include "configuration.h"

void keyboard_handler(const unsigned char key, int a, int b)
{
	if (key == key_escape)
	{
		glutExit();
		return;
	}
	
	camera::get_instance()->handle_keyboard_input(key);
}

void mouse_wheel_handler(const int button, const int direction, const int position_x, const int position_y)
{
	camera::get_instance()->handle_mouse_wheel_input(button, direction, position_x, position_y);
}

void mouse_handler(const int position_x, const int position_y)
{
    camera::get_instance()->handle_mouse_input(position_x, position_y, width / 2, height / 2);
}
