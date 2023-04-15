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

	// Position: x:-4.29697, y: 15.7, z: 18.95 Target: x:0.221551, y: -0.559194, z: -0.798885 YAW: -74.4999 PITCH: -34.0001 FOV: 45
	glm::vec3 default_drone_position = glm::vec3(-4.2, 15.7, 18.95);
	glm::vec3 default_drone_front_position = glm::vec3(0, 0, 0);
	glm::vec3 default_drone_up_position = glm::vec3(0, 1, 0);
	glm::vec3 default_walk_position = glm::vec3(0, 5.2, 13.0);
	glm::vec3 default_walk_front_position = glm::vec3(0, 0, 0);
	glm::vec3 default_walk_up_position = glm::vec3(0, 1, 0);
	glm::vec3 *position = new glm::vec3(-4.2, 15.7, 18.95); // x, y, z
	glm::vec3 *front = new glm::vec3(0, 0, 0); // x, y, z
	glm::vec3 *up = new glm::vec3(0, 1, 0); // x, y, z

	// The yaw values determines if we are looking left/right.
	const float default_drone_yaw = -74.5f;
	const float default_walk_yaw = -86.0f;
	float yaw = default_drone_yaw;

	// The pitch values determines if we are looking up/down.
	const float default_drone_pitch = -34.0f;
	const float default_walk_pitch = -7.4f;
	float pitch = default_drone_pitch;

	// Field of view (fov) largely defines how much we can see of the scene.
	const float default_fov = 45.0f;
	const float min_fov = 1.0f;
	const float max_fov = 90.0f;
	float fov = default_fov;

	const float minimum_walk_mode_position_y = 3.2f;
	bool is_drone_mode_enabled = true;

	static camera* get_instance();
	~camera();

	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
	void update() const;
	void update_after_yaw_or_pitch_change() const;

	void handle_keyboard_input(unsigned char key);
	void handle_mouse_input(int position_x, int position_y, int x_center, int y_center);
	void handle_mouse_wheel_input(const int button, const int direction, const int position_x, const int position_y);
};
