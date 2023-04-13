#include "animation_rotate.h"

#include "matrix_transformations.h"

animation_rotate::animation_rotate(const float rotate_speed, const float x, const float y, const float z)
{
	rotate_speed_ = rotate_speed;
	rotate_x_ = x;
	rotate_y_ = y;
	rotate_z_ = z;
}

animation_rotate::animation_rotate(const float rotate_speed, const float rotate_value)
: animation_rotate(rotate_speed, rotate_value, rotate_value, rotate_value)
{
}

glm::mat4 animation_rotate::execute(const glm::mat4& model) const
{
	return matrix_rotate(model, rotate_speed_, rotate_x_, rotate_y_, rotate_z_);
}
