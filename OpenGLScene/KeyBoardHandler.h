#pragma once

enum supported_keys
{
	key_escape = 27,
	key_w_lower = 119,
	key_w_upper = 87,
	key_s_lower = 115,
	key_s_upper = 83,
	key_a_lower = 97,
	key_a_upper = 65,
	key_b_lower = 66,
	key_b_upper = 98,
	key_d_lower = 100,
	key_d_upper = 68
};

void keyboard_handler(const unsigned char key, int a, int b);
void mouse_wheel_handler(const int button, const int direction, const int position_x, const int position_y);
void mouse_handler(int position_x, int position_y);