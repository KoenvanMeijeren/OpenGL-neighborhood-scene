#include "Camera.h"

#include <iostream>
#include <GL/freeglut_std.h>

#include "configuration.h"
#include <glm/gtc/matrix_transform.hpp>

#include "KeyBoardHandler.h"

camera* camera::instance_ { nullptr };

camera::camera()
{
	this->view = new glm::mat4();
	*this->view = glm::lookAt(
		*this->position,
		*this->position + *this->front,
		*this->up
	);
	this->projection = new glm::mat4();
	*this->projection = glm::perspective(
		glm::radians(this->fov),
		1.0f * width / height, 
		0.1f,
		100.0f
	);
}

camera* camera::get_instance()
{
	if (camera::instance_ == nullptr)
	{
		camera::instance_ = new camera();
		std::cout << "New camera instance created\n";
		return camera::instance_;
	}

	return camera::instance_;
}

camera::~camera()
{
	delete this->view;
	delete this->projection;
	delete this->position;
	delete this->front;
	delete this->up;
	delete camera::instance_;
}

glm::mat4 camera::get_view() const
{
	return *this->view;
}

glm::mat4 camera::get_projection() const
{
	return *this->projection;
}

void camera::update() const
{
	*this->view = glm::lookAt(
		*this->position,
		*this->position + *this->front,
		*this->up
	);

	*this->projection = glm::perspective(
		glm::radians(this->fov),
		1.0f * width / height, 
		0.1f,
		100.0f
	);
}

void camera::handle_keyboard_input(const unsigned char key) const
{
	constexpr float camera_speed = 0.5f;
	switch (key)
	{
	case key_w_lower:
	case key_w_upper:
		*this->position += *this->front * camera_speed;
		break;
	case key_s_lower:
	case key_s_upper:
		*this->position -= *this->front * camera_speed;
		break;
	case key_a_lower:
	case key_a_upper:
		*this->position -= glm::normalize(glm::cross(*this->front, *this->up)) * camera_speed;
		break;
	case key_d_lower:
	case key_d_upper:
		*this->position += glm::normalize(glm::cross(*this->front, *this->up)) * camera_speed;
		break;
	case key_b_lower:
	case key_b_upper:
		std::cout << "Position: x:" << this->position->x << ", y: " << this->position->y << ", z: " << this->position->z;
		std::cout << " Target: x:" << this->front->x << ", y: " << this->front->y << ", z: " << this->front->z;
		std::cout << " YAW: " << this->yaw;
		std::cout << " PITCH: " << this->pitch;
		std::cout << " FOV: " << this->fov;
		std::cout << "\n";
		break;
	}
}

// Implementation based on the tutorial provided by: https://learnopengl.com/Getting-started/Camera
void camera::handle_mouse_input(const int position_x, const int position_y, const int x_center, const int y_center)
{
	constexpr float max_mouse_sensitivity = 0.1f;
	float mouse_moved_offset_x = static_cast<float>(position_x) - static_cast<float>(x_center);
	float mouse_moved_offset_y = static_cast<float>(y_center) - static_cast<float>(position_y);
	mouse_moved_offset_x *= max_mouse_sensitivity;
	mouse_moved_offset_y *= max_mouse_sensitivity;

	// Update the properties which are used for changing the camera to a new perspective.
	this->yaw += mouse_moved_offset_x;
	this->pitch += mouse_moved_offset_y;

	// Prevent rotating through the scene.
	constexpr float maximum_pitch_right_direction = 89.0f;
	if (this->pitch > maximum_pitch_right_direction)
	{
		this->pitch = maximum_pitch_right_direction;
	}
	else if (this->pitch < -maximum_pitch_right_direction)
	{
		this->pitch = -maximum_pitch_right_direction;
	}

	// Update the camera position, front and view.
	glm::vec3 new_target_direction;
	new_target_direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	new_target_direction.y = sin(glm::radians(this->pitch));
	new_target_direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	*this->front = glm::normalize(new_target_direction);

	this->update();

	// Prevent exiting the center of the screen.
	if (position_x != x_center || position_y != y_center) 
	{
		glutWarpPointer(x_center, y_center);
	}
}

// TODO: Find out how to make this working with uniform variables
// TODO: Find out why the scene is not updated after scrolling
void camera::handle_mouse_wheel_input(const int button, const int direction, const int position_x, const int position_y)
{
	if (direction > 0)
    {
        // Zoom in
		this->fov += 1.0f;
    }
    else
    {
        // Zoom out
		this->fov -= 1.0f;
    }

    if (fov < min_fov)
    {
	    fov = 1.0f;
    }

    if (fov > max_fov)
    {
	    fov = 45.0f;
    }
}
