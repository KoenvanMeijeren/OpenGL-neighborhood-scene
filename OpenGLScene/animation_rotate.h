#pragma once

#include "animation.h"

class animation_rotate final : public animation
{
private:
	float rotate_speed_ = 0, rotate_x_ = 0, rotate_y_ = 0, rotate_z_ = 0;
public:
	explicit animation_rotate() = default;
	explicit animation_rotate(const float rotate_speed, const float x, const float y, const float z);
	explicit animation_rotate(const float rotate_speed, const float rotate_value);
	glm::mat4 execute(const glm::mat4& model) override;
};
