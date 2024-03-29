﻿#pragma once
#include <malloc.h>

#include "animation.h"

__declspec(align(16)) class animation_move_in_range final : public animation
{
private:
	bool original_model_initialized_ = false;
	glm::mat4 original_model_{};
	float movement_speed_ = 0;
	float current_position_x_ = 0, current_position_y_ = 0, current_position_z_ = 0;
	float position_x_max_ = 0, position_y_max_ = 0, position_z_max_ = 0;
public:
	explicit animation_move_in_range(const float movement_speed, const float position_x_max_value, const float position_y_max_value, const float position_z_max_value);

	void* operator new(const size_t size)
	{
	    return _mm_malloc(size, 16);
	}

	void operator delete(void* pointer)
	{
	    _mm_free(pointer);
	}

	glm::mat4 execute(const glm::mat4& model) override;
};
