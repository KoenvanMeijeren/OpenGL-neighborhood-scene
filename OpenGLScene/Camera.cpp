#include "Camera.h"

#include <iostream>
#include <GL/freeglut_std.h>

#include "configuration.h"
#include <glm/gtc/matrix_transform.hpp>

#include "KeyBoardHandler.h"

camera* camera::instance_ { nullptr };

camera::camera()
{
	this->view = static_cast<glm::mat4*>(_aligned_malloc(sizeof(glm::mat4), 16));
	*this->view = glm::lookAt(
		*this->position,
		*this->position + *this->front,
		*this->up
	);
	this->projection = static_cast<glm::mat4*>(_aligned_malloc(sizeof(glm::mat4), 16));
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

void camera::update_after_yaw_or_pitch_change() const
{
	// Update the camera position, front and view.
	glm::vec3 new_target_direction;
	new_target_direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	new_target_direction.y = sin(glm::radians(this->pitch));
	new_target_direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	*this->front = glm::normalize(new_target_direction);
}

void camera::handle_keyboard_input(const unsigned char key)
{
	constexpr float camera_speed = 0.5f;
	switch (key)
	{
	case key_w_lower:
	case key_w_upper:
		// Move forwards.
		*this->position += *this->front * camera_speed;
		break;
	case key_s_lower:
	case key_s_upper:
		// Move backwards.
		*this->position -= *this->front * camera_speed;
		break;
	case key_a_lower:
	case key_a_upper:
		// Move rightwards.
		*this->position += glm::normalize(glm::cross(*this->front, *this->up)) * camera_speed;
		break;
	case key_d_lower:
	case key_d_upper:
		// Move leftwards.
		*this->position -= glm::normalize(glm::cross(*this->front, *this->up)) * camera_speed;
		break;
	case key_i_lower:
	case key_i_upper:
		// Look more upwards.
		this->pitch += camera_speed;
		this->update_after_yaw_or_pitch_change();
		break;
	case key_k_lower:
	case key_k_upper:
		// Look more downwards. 
		this->pitch -= camera_speed;
		this->update_after_yaw_or_pitch_change();
		break;
	case key_j_lower:
	case key_j_upper:
		// Look more leftwards.
		this->yaw -= camera_speed;
		this->update_after_yaw_or_pitch_change();
		break;
	case key_l_lower:
	case key_l_upper:
		// Look more rightwards.
		this->yaw += camera_speed;
		this->update_after_yaw_or_pitch_change();
		break;
	case key_e_lower:
	case key_e_upper:
		// Move upwards.
		if (!this->is_drone_mode_enabled)
		{
			return;
		}

		this->position->y += camera_speed;
		break;
	case key_q_lower:
	case key_q_upper:
		// Move downwards.
		if (!this->is_drone_mode_enabled)
		{
			return;
		}

		this->position->y -= camera_speed;
		break;
	case key_v_lower:
	case key_v_upper:
		this->is_drone_mode_enabled = !this->is_drone_mode_enabled;

		this->yaw = default_drone_yaw;
		this->pitch = default_drone_pitch;
		*this->position = default_drone_position;
		*this->front = default_drone_front_position;
		*this->up = default_drone_up_position;
		if (!this->is_drone_mode_enabled)
		{
			this->yaw = default_walk_yaw;
			this->pitch = default_walk_pitch;
			*this->position = default_walk_position;
			*this->front = default_walk_front_position;
			*this->up = default_walk_up_position;
		}

		if (!this->is_drone_mode_enabled)
		{
			this->front->y = 0;
		}

		// Prevent falling through the floor when not in drone mode.
		if (!this->is_drone_mode_enabled && this->position->y < minimum_walk_mode_position_y)
		{
			this->position->y = minimum_walk_mode_position_y;
		}

		this->update_after_yaw_or_pitch_change();
		this->update();
		break;
	case key_b_lower:
	case key_b_upper:
		std::cout << "Position: x:" << this->position->x << ", y: " << this->position->y << ", z: " << this->position->z;
		std::cout << " Target: x:" << this->front->x << ", y: " << this->front->y << ", z: " << this->front->z;
		std::cout << " YAW: " << this->yaw;
		std::cout << " PITCH: " << this->pitch;
		std::cout << " FOV: " << this->fov;
		std::cout << " Drone mode enabled: " << (this->is_drone_mode_enabled ? "Yes" : "No");
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

	this->update_after_yaw_or_pitch_change();
	this->update();

	// Prevent exiting the center of the screen.
	if (position_x != x_center || position_y != y_center) 
	{
		glutWarpPointer(x_center, y_center);
	}
}

void camera::handle_mouse_wheel_input(const int button, const int direction, const int position_x, const int position_y)
{
	if (direction > 0)
    {
        // Zoom in
		this->fov -= 1.0f;
    }
    else
    {
        // Zoom out
		this->fov += 1.0f;
    }

    if (fov < min_fov)
    {
	    fov = min_fov;
    }

    if (fov > max_fov)
    {
	    fov = max_fov;
    }
}
