#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

camera::camera(const float width, const float height)
{
	const auto temporary_mat4_value = new glm::mat4();
	this->view = temporary_mat4_value;
	*this->view = glm::lookAt(
		this->lookAtPosition,
		this->lookAtPosition + this->pointAtPosition,
		this->upDirection
	);
	this->projection = temporary_mat4_value;
	*this->projection = glm::perspective(
		glm::radians(45.0f),
		1.0f * width / height, 0.1f,
		20.0f
	);
	delete temporary_mat4_value;
}
;

camera::~camera()
{
	delete this->view;
	delete this->projection;
};

glm::mat4 camera::get_projection() const
{
	return *this->projection;
}

glm::mat4 camera::get_view() const
{
	return *this->view;
}

glm::vec3 camera::get_look_at_position()
{
	return this->lookAtPosition;
}

glm::vec3 camera::get_point_at_position()
{
	return this->pointAtPosition;
}

glm::vec3 camera::get_up_direction()
{
	return this->upDirection;
}

float camera::get_yaw_up_orientation() const
{
	return this->yawUpOrientation;
}

float camera::get_pitch_right_direction() const
{
	return this->pitchRightDirection;
}

void camera::enable_drone_mode()
{
	this->drone_mode_enabled = true;
}

void camera::disable_drone_mode()
{
	this->drone_mode_enabled = false;
}

bool camera::is_drone_mode_enabled() const
{
	return this->drone_mode_enabled;
}
