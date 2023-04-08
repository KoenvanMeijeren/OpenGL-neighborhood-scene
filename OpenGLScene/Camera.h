#pragma once
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

class camera
{
private:
	glm::vec3 lookAtPosition = glm::vec3(0, 2, 8); // x, y, z
	glm::vec3 pointAtPosition = glm::vec3(0, -2, -8); // x, y, z

	// The Y axis is the up direction.
	glm::vec3 upDirection = glm::vec3(0, 1, 0); // x, y, z

	// The yaw values determines if we are looking left/right.
	float yawUpOrientation = 0;

	// The pitch values determines if we are looking up/down.
	float pitchRightDirection = 0;

	bool drone_mode_enabled = true;
public:
	glm::mat4 *projection, *view;

	camera(const float width, const float height);
	~camera();

	glm::mat4 get_projection() const;
	glm::mat4 get_view() const;

	glm::vec3 get_look_at_position();
	glm::vec3 get_point_at_position();
	glm::vec3 get_up_direction();
	float get_yaw_up_orientation() const;
	float get_pitch_right_direction() const;
	void enable_drone_mode();
	void disable_drone_mode();
	bool is_drone_mode_enabled() const;
};
