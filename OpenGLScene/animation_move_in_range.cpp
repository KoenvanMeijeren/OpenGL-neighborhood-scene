#include "animation_move_in_range.h"

#include <iostream>

#include "matrix_transformations.h"

animation_move_in_range::animation_move_in_range(const float movement_speed, const float position_x_max_value, const float position_y_max_value,
	const float position_z_max_value)
{
	movement_speed_ = movement_speed;
	position_x_max_ = position_x_max_value;
	position_y_max_ = position_y_max_value;
	position_z_max_ = position_z_max_value;
}

glm::mat4 animation_move_in_range::execute(const glm::mat4& model)
{
	if (!original_model_initialized_)
	{
		original_model_ = model;
		original_model_initialized_ = true;
		std::cout << "Initialized model\n";
	}

	bool has_reset_value = false;
	float translate_position_x = 0;
	if (position_x_max_ < 0 || position_x_max_ > 0)
	{
		translate_position_x = movement_speed_;
		current_position_x_ += movement_speed_;
		if (current_position_x_ > position_x_max_)
		{
			current_position_x_ = 0;
			has_reset_value = true;
			std::cout << "Reset after x change\n";
		}
	}

	float translate_position_y = 0;
	if (position_y_max_ < 0 || position_y_max_ > 0)
	{
		translate_position_y = movement_speed_;
		current_position_y_ += movement_speed_;
		if (current_position_y_ > position_y_max_)
		{
			current_position_y_ = 0;
			has_reset_value = true;
			std::cout << "Reset after y change\n";
		}
	}

	float translate_position_z = 0;
	if (position_z_max_ < 0 || position_z_max_ > 0)
	{
		translate_position_z = movement_speed_;
		current_position_z_ += movement_speed_;
		std::cout << "Change z position\n";
		if (current_position_z_ > position_z_max_)
		{
			current_position_z_ = 0;
			has_reset_value = true;
			std::cout << "Reset after z change\n";
		}
	}

	if (has_reset_value)
	{
		std::cout << "Reset model";
		return original_model_;
	}

	return matrix_translate(model, translate_position_x, translate_position_y, translate_position_z);
}
