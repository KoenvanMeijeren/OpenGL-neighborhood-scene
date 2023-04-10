#pragma once
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

// Structure inspired by http://tuttlem.github.io/2013/12/30/a-camera-implementation-in-c.html
class camera
{
private:
	static camera* instance_;

	camera();
public:
	glm::mat4 *projection;
	glm::mat4 *view;

	glm::vec3 *position = new glm::vec3(0, 3.2, 13.8); // x, y, z
	glm::vec3 *front = new glm::vec3(0, 0, 0); // x, y, z
	glm::vec3 *up = new glm::vec3(0, 1, 0); // x, y, z

	// The yaw values determines if we are looking left/right.
	const float default_yaw = -86.0f;
	float yaw = default_yaw;

	// The pitch values determines if we are looking up/down.
	const float default_pitch = -12.4f;
	float pitch = default_pitch;

	// Field of view or fov largely defines how much we can see of the scene.
	const float default_fov = 45.0f;
	const float min_fov = 1.0f;
	const float max_fov = 90.0f;
	float fov = default_fov;

	bool is_drone_mode_enabled = true;

	static camera* get_instance();
	~camera();

	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
	void update() const;

	void handle_keyboard_input(unsigned char key) const;
	void handle_mouse_input(int position_x, int position_y, int x_center, int y_center);
	void handle_mouse_wheel_input(const int button, const int direction, const int position_x, const int position_y);
};
